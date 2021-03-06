#include "spell_checker.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <unordered_set>
#include <fstream>
#include <exception>

namespace {
	std::string string_to_lower(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
}

template<template <typename... Args> class Container>
class SpellChecker_Container : public SpellChecker_Impl
{
public:
	void load(const std::string &) override;
	bool check (const std::string &) const override;
	void add (const std::string &) override;
	size_t size(void) const override;

private:
	Container<std::string> storage_;
};

template<template <typename... Args> class Container>
void SpellChecker_Container<Container>::load(const std::string &dictionary) {
	std::ifstream file;
	file.open(dictionary);
	if (!file.is_open()) {
		throw SpellChecker_InvalidDictFile();
	}
	std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(),
											  std::inserter(storage_, storage_.end()));
	file.close();
}

template<>
void SpellChecker_Container<std::vector>::load(const std::string &dictionary) {
	std::ifstream file;
	file.open(dictionary);
	if (!file.is_open()) {
		throw SpellChecker_InvalidDictFile();
	}
	std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(),
											   std::back_inserter(storage_));
	file.close();
}

template<template <typename... Args> class Container>
size_t SpellChecker_Container<Container>::size(void) const {
	  return storage_.size();
}

template<template <typename... Args> class Container>
bool SpellChecker_Container<Container>::check(const std::string &word) const {
	if (storage_.find(string_to_lower(word)) == storage_.end()) {
		return false;
	}
	return true;
}

template <>
bool SpellChecker_Container<std::vector>::check(const std::string &word) const {
	return std::binary_search(std::begin(storage_), std::end(storage_), string_to_lower(word));
}

template<template <typename... Args> class Container>
void SpellChecker_Container<Container>::add(const std::string& word) {
	if (!check(word)) {
		storage_.insert(word);
	}
}

template<>
void SpellChecker_Container<std::vector>::add(const std::string& word) {
	if (!SpellChecker_Container<std::vector>::check(word)) {
		storage_.insert(std::upper_bound(storage_.begin(), storage_.end(), word), word);
	}
}


SpellChecker::SpellChecker(const ContainerType type) {
	if (type == ContainerType::Vector) {
		impl_ = std::make_unique<SpellChecker_Container<std::vector>>();
	}
	else if (type == ContainerType::Set) {
		impl_ = std::make_unique<SpellChecker_Container<std::set>>();
	}
	else if (type == ContainerType::Unordered_Set) {
		impl_ = std::make_unique<SpellChecker_Container<std::unordered_set>>();
	}
	else {
		impl_ = nullptr;
	}
}

void SpellChecker::load(const std::string &dictionary) {
	impl_->load(dictionary);
}

bool SpellChecker::check(const std::string &word) const {
	return impl_->check(word);
}

void SpellChecker::add(const std::string& word) {
	impl_->add(word);
}

size_t SpellChecker::size(void) const {
	return impl_->size();
}

bool SpellChecker::is_valid(const std::string &word) {
	if (word.size() > 45 || *word.begin() == '\'') {
		return false;
	}
	for (auto it : word) {
		if (!std::isalpha(it) && it != '\'') {
				return false;
		}
	}
	return true;
}