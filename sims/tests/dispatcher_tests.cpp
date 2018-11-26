#include <lp3/core.hpp>
#include <lp3/sims/dispatcher.hpp>
#include <catch2/catch.hpp>

namespace sims = lp3::sims;


TEST_CASE("Test Dispatching", "[dispatcher_tests]") {
	lp3::core::LogSystem log;
	sims::EventDispatcher dispatcher;
	sims::EventType ATTACKED = 1;
	struct AttackInfo {
		int hp;
		std::string character_id;
	};
	auto on_attack = [](const AttackInfo & info) {
		(void)info;
		LP3_LOG_INFO("Character %d was attacked for %d damage.",
			info.character_id, info.hp);
	};
	dispatcher.subscribe<AttackInfo>(ATTACKED, on_attack);
	dispatcher.send(ATTACKED, AttackInfo{ 35, "Alaghasta the Ninny" });

	dispatcher.subscribe<AttackInfo>(ATTACKED, [](const AttackInfo & info) {
		(void)info;
		LP3_LOG_INFO("I also saw that %s was just attacked for %d damage!",
			info.character_id, info.hp);
	});
	dispatcher.send<AttackInfo>(ATTACKED, { 35, "Burtman the Offender" });

	//dispatcher.subscribe<int>(ATTACKED, [](const int & info) {
	//	LP3_LOG_INFO("I am a bug. %d", info);
	//});

	{
		lp3::core::AssertListener listen;
		LP3_REQUIRE_ASSERT_FAILURE(dispatcher.send<int>(ATTACKED, 5));
	}

	{
		lp3::core::AssertListener listen;
		LP3_REQUIRE_ASSERT_FAILURE(
			dispatcher.subscribe<int>(ATTACKED, [](const int & info) {
				(void)info;
				LP3_LOG_INFO("I am a bug. %d", info);
			}));
	}
}


TEST_CASE("Unsubscribe", "[dispatcher_unsubscribe]") {
	lp3::core::LogSystem log;
	sims::EventDispatcher dispatcher;
	sims::EventType ATTACKED = 1;
	int my_hp = 100;
	auto on_attack = [&my_hp](const int hp) {
		LP3_LOG_INFO("HURT! HP -%d", hp);
		my_hp -= hp;
	};

	sims::SubscriptionId id = dispatcher.subscribe<int>(ATTACKED, on_attack);
	dispatcher.send(ATTACKED, 35);
	REQUIRE(65 == my_hp);

	dispatcher.unsubscribe(id);
	dispatcher.send(ATTACKED, 10);
	REQUIRE(55 == my_hp);  // What! still changed?!!!/*

	dispatcher.prune();
	dispatcher.send(ATTACKED, 10);
	REQUIRE(55 == my_hp);  // Ah, no change. Just have to prune.*/
}

