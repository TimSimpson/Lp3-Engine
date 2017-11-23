#define LP3_TEST_MODULE coroutine_tests
#include <lp3/sims.hpp>
#include <catch/catch.hpp>

namespace sims = lp3::sims;

struct Coroutine1
{
    sims::CoroutineState state;
    int i;
    int result;

    Coroutine1()
    :   result(0)
    {
    }

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
            LP3_LABEL(start_loop);
                if (i >= 100) {
                    goto end_loop;
                }
                LP3_YIELD();
                ++ i;
                goto start_loop;

            LP3_LABEL(end_loop);
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
