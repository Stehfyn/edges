#include "edges.hpp"

#include <string>
#include <thread>
#include <iostream>

void test_seq() {
	std::cout << "Testing sequence ============================================" << std::endl;

	auto seq = edges::make_sequence(
		edges::predicate_wrapper([](const int& i) {
			return i == 1;
		}),
		edges::predicate_wrapper([](const std::string_view& str) {
			return str == "cool";
		})
	);

	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "try_advance (2): " << seq.try_advance(2) << std::endl;
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl << std::endl;
}

void test_seq_with_timeout() {
	std::cout << "Testing sequence with timeout (3 seconds) ===================" << std::endl;

	auto seq = edges::make_sequence_with_config(
		edges::sequence_config {
			edges::sequence_policy::weak,
			edges::timeout_config {
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(3)),
				edges::timeout_policy::failure
			}
		},
		edges::predicate_wrapper([](const int& i) {
			return i == 1;
		}),
		edges::predicate_wrapper([](const std::string_view& str) {
			return str == "cool";
		})
	);

	std::cout << "Attempt 1 ===================================================" << std::endl;
	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "try_advance (2): " << seq.try_advance(2) << std::endl;
	std::cout << "Sleeping for 4 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl;
	std::cout << "duration (s): " << seq.elapsed() << std::endl;

	std::cout << "Attempt 2 ===================================================" << std::endl;
	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "try_advance (2): " << seq.try_advance(2) << std::endl;
	std::cout << "Sleeping for 2 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl;
	std::cout << "duration (s): " << seq.elapsed() << std::endl;
}
void test_strict_seq_with_timeout() {
	std::cout << "Testing strict sequence with timeout (3 seconds) ===================" << std::endl;

	auto seq = edges::make_sequence_with_config(
		edges::sequence_config {
			edges::sequence_policy::strict,
			edges::timeout_config {
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(3)),
				edges::timeout_policy::failure
			}
		},
		edges::predicate_wrapper([](const int& i) {
			return i == 1;
		}),
		edges::predicate_wrapper([](const std::string_view& str) {
			return str == "cool";
		})
	);

	std::cout << "Attempt 1 ===================================================" << std::endl;
	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "try_advance (2): " << seq.try_advance(2) << std::endl;
	std::cout << "Sleeping for 4 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl;
	std::cout << "duration (s): " << seq.elapsed() << std::endl;

	std::cout << "Attempt 2 ===================================================" << std::endl;
	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "try_advance (2): " << seq.try_advance(2) << std::endl;
	std::cout << "Sleeping for 2 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl;
	std::cout << "duration (s): " << seq.elapsed() << std::endl;

	std::cout << "Attempt 3 ===================================================" << std::endl;
	std::cout << "try_advance (1): " << seq.try_advance(1) << std::endl;
	std::cout << "Sleeping for 2 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "try_advance (\"cool\"): " << seq.try_advance(std::string_view("cool")) << std::endl;
	std::cout << "is_complete (): " << seq.is_complete() << std::endl;
	std::cout << "duration (s): " << seq.elapsed() << std::endl;
}
int main() {
	test_seq();
	test_seq_with_timeout();
	test_strict_seq_with_timeout();
}