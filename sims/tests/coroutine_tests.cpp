#define LP3_TEST_MODULE coroutine_tests
#include <lp3/sims.hpp>
#include <catch/catch.hpp>

namespace sims = lp3::sims;

struct Coroutine1
{
    sims::CoroutineState state;
    int result;

    Coroutine1()
    :   result(0)
    {}

    void operator()() {
        LP3_COROUTINE_BEGIN(state);
        result = 50;
        LP3_YIELD();
        result = 100;
        LP3_COROUTINE_END();
    }
};

TEST_CASE("Should catch fall off", "[should_catch_falloff]") {
    Coroutine1 co;
	REQUIRE(0 == co.result);
    co();
    REQUIRE(50 == co.result);
    co();
    REQUIRE(100 == co.result);
    REQUIRE_THROWS_AS(co(), sims::CoroutineFinishedException);
}

TEST_CASE("Should catch invalid state", "[should_catch_invalid_state]") {
    Coroutine1 co;
    co.state.code_pointer = -434;
    REQUIRE_THROWS_AS( co(), sims::CoroutineInvalidStateException);
}

TEST_CASE("Running handcranked loop", "[running_handcranked_loop]") {
    struct CO{
        sims::CoroutineState state;
        int i = -1;
        bool finished = false;

        void operator() () {
            LP3_COROUTINE_BEGIN(state);

            i = 0;
            start_loop:
                if (i >= 100) {
                    goto end_loop;
                }
                LP3_YIELD();
                ++ i;
                goto start_loop;

            end_loop:
                finished = true;
                LP3_YIELD();

            LP3_COROUTINE_END();
        }
    } co;

    REQUIRE(co.finished == false);
    int lastI = co.i;
    while(!co.finished)
    {
        co();
        REQUIRE(lastI < co.i);
    }
    REQUIRE(co.i == 100);
}

TEST_CASE("example", "[example]") {
// ----------------------------------------------------------------
// This example shows how coroutines can make writing the logic
// for game entities that update once per frame easier.
//
// Here we create a turtle, that simply moves 640 units to the right
// before moving back to the left (assume ``operator() ()`` gets
// called once per frame in some kind of game loop).
// ----------------------------------------------------------------
    struct Turtle{
        int x;
        sims::CoroutineState state;

        Turtle()
        :   x(0),
            state()
        {}

        void operator() (const std::int64_t elapsed_ms) {
            // we want to move at 100 of whatever units x is in a second
            const int speed = lp3::narrow<int>(100 * (elapsed_ms / 1000.0));
            LP3_LOG_ERROR("speed = %d", speed);
            LP3_COROUTINE_BEGIN(state);
                while(true) {
                    x += speed;
                    if (x >= 640) {
                        break;
                    }
                    LP3_YIELD();
                }

                if (x >= 640) {
                    x = 640 - (x - 640);
                }
                LP3_YIELD();

                while(true) {
                    x -= speed;
                    if (x < 0) {
                        x = 0;
                        break;
                    }
                    LP3_YIELD();
                }
            LP3_COROUTINE_END();
        }

        explicit inline operator bool() const {
            return (bool) state;
        }
    } turtle;

    // the turtle starts at 0
    REQUIRE(turtle.x == 0);
    // this means the coroutine is not finished and is safe to run
    REQUIRE((bool) turtle.state == true);
    // if it moves at 100 per second, it will be at 300 in 3 seconds
    turtle(3000);
    REQUIRE(turtle.x == 300);
    // in 3 more seconds, it's at 600..
    turtle(3000);
    REQUIRE(turtle.x == 600);
    // in 3 more seconds, it gets to 640 and turns around, ending up at 380
    turtle(3000);
    REQUIRE(turtle.x == 380);
    // in  3.8 seconds it will slam back into zero and stop-
    turtle(3800);
    REQUIRE(turtle.x == 0);
    turtle(1000);
    // The coroutine is now finished. If we call it again, it
    // will raise an exception.
    REQUIRE((bool) turtle.state == false);
    REQUIRE_THROWS_AS(turtle(1000), sims::CoroutineFinishedException);
// ~end-doc
}

TEST_CASE("loop de loop", "[running_loop_de_loop]") {
    struct CO{
        sims::CoroutineState state;
        int forCode_pointer = 0;
        int i = -1;
        bool finished = false;
        std::vector<char> letters;

        void operator() () {
            LP3_COROUTINE_BEGIN(state);
                for (i = 0; i < 5; i ++)
                {
                    letters.push_back('a');
                    LP3_YIELD();
                    letters.push_back('b');
                    LP3_YIELD();
                    letters.push_back('c');
                }
                LP3_YIELD();
                finished = true;
            LP3_COROUTINE_END();
        }

        explicit inline operator bool() const {
            return (bool) state;
        }
    } co;

    REQUIRE(co.finished == false);
    REQUIRE((bool) co == true);
    REQUIRE(co.i == -1);
    REQUIRE(co.letters.size() == std::size_t{0});
    co(); // starts- sets i to 0 and adds 'a'
    REQUIRE(co.i == 0);
    REQUIRE(co.letters.size() == std::size_t{1});
    co(); // adds 'b'
    REQUIRE(co.i == 0);
    REQUIRE(co.letters.size() == std::size_t{2});
    co();   // now adds 'c' == increments i and adds 'a'
    REQUIRE(co.i == 1);
    REQUIRE(co.letters.size() == std::size_t{4});

    CO co2; // make a new one so we aren't as confused.
    // Mimic the inner loop
    std::size_t letterCount = 0;
    for (int i = 0; i < 5; i ++) {
        co2();
        ++ letterCount;
        REQUIRE(co2.i == i);
        REQUIRE(co2.letters.size() == letterCount);
        co2();
        ++ letterCount;
        REQUIRE(co2.i == i);
        REQUIRE(co2.letters.size() == letterCount);
        // Next loop, we'll get one more letter, so increment letterCount here.
        ++ letterCount;
    }
    REQUIRE(co2.finished == false);
    REQUIRE((bool) co2 == true);
    co2();
    // It will add a letter, then exit the loop, but yield.
    REQUIRE(co2.i == 5);
    REQUIRE(co2.letters.size() == std::size_t{3 * 5});
    REQUIRE(co2.finished == false);
    REQUIRE((bool) co2 == true);
    // Now it will end the coroutine.
    co2();
    REQUIRE(co2.i == 5);
    REQUIRE(co2.letters.size() == std::size_t{3 * 5});


    REQUIRE(co2.finished == true);
    REQUIRE(co2.state.code_pointer == -1);
    REQUIRE((bool) co2 == false);
}


TEST_CASE("simple generator", "[simple_generator]") {
     struct Numbers{
        sims::CoroutineState state;
        int number = 0;

        int operator() () {
            LP3_COROUTINE_BEGIN(state)
                while(true)
                {
                    LP3_YIELD(number);
                    ++ number;
                }
            LP3_COROUTINE_END();
        }
    } numbers;
    REQUIRE(numbers() == 0);
    REQUIRE(numbers() == 1);
    REQUIRE(numbers() == 2);
    REQUIRE(numbers() == 3);
}
