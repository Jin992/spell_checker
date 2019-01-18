#include "spell_checker.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <unordered_set>
#include <fstream>
#include <exception>
#include <regex>

namespace {
	std::string string_to_lower(std::string str) {
    	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    	return str;
	}
}

template<template <typename... Args> class Container, typename... T>
SpellChecker_Container<Container, T...>::SpellChecker_Container()
: dict_size_(0)
{}

template<template <typename... Args> class Container, typename... T>
void SpellChecker_Container<Container, T...>::load(const std::string &dictionary) {
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
void SpellChecker_Container<std::vector, std::string>::load(const std::string &dictionary) {
	std::ifstream file;
	file.open(dictionary);
	if (!file.is_open()) {
		throw SpellChecker_InvalidDictFile();
	}
	std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(),
											   std::back_inserter(storage_));
	file.close();
}

template<template <typename... Args> class Container, typename... T>
size_t SpellChecker_Container<Container, T...>::size(void) const {
	  return storage_.size();
}

template<template <typename... Args> class Container, typename... T>
bool SpellChecker_Container<Container, T...>::check(const std::string &word) const {
 	if (storage_.find(string_to_lower(word)) == storage_.end()) {
 		return false;
 	}
 	return true;
 }

template <>
bool SpellChecker_Container<std::vector, std::string>::check(const std::string &word) const {
 	return std::binary_search(std::begin(storage_), std::end(storage_), string_to_lower(word));
 }

SpellChecker::SpellChecker(const ContainerType type) 
{
	//std::cout << "CONTEINER ENUM " << ContainerType::Vector << std::endl;
	if (type == ContainerType::Vector) {
		impl_ = std::make_unique<SpellChecker_Container<std::vector, std::string>>();
	}
	else if (type == ContainerType::Set) {
		impl_ = std::make_unique<SpellChecker_Container<std::set, std::string>>();
	}
	else if (type == ContainerType::Unordered_Set) {
		impl_ = std::make_unique<SpellChecker_Container<std::unordered_set, std::string>>();
	}
}

void SpellChecker::load(const std::string &dictionary) {
	impl_->load(dictionary);
}

bool SpellChecker::check(const std::string &word) const {
	return impl_->check(word);
}

size_t SpellChecker::size(void) const {
	return impl_->size();
}

bool SpellChecker::is_valid(const std::string &word) {;
	const std::regex regex_cond("[a-zA-Z']*");
 	if (word.size() > 45 || !std::regex_match(word.begin(), word.end(), regex_cond)) {
 		return false;
 	}
 	if (*word.begin() == '\'') {
 		return false;
 	}
	return true;
}




