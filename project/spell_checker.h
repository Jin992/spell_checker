#pragma once

#include <memory>
#include <string>

// exception on failure to load dictionary file
class SpellChecker_InvalidDictFile {} ;

//class SpellChecker_Impl
class SpellChecker_Impl
{
public:
   virtual void load (const std::string& dictionary) = 0;
   virtual bool check (const std::string& word) const = 0;
   virtual void add (const std::string& word) = 0;
   virtual size_t size(void) const = 0;
   virtual ~SpellChecker_Impl() {}
};

template<template <typename... Args> class Container, typename... T>
class SpellChecker_Container : public SpellChecker_Impl
{
public:
   SpellChecker_Container();
   void load(const std::string &dictionary) override;
   bool check (const std::string &word) const override;
   void add (const std::string& word) override;
   size_t size(void) const override;

private:
   Container<T...> storage_;
   size_t dict_size_;
};

enum class ContainerType { Vector, Set, Unordered_Set };

class SpellChecker
{
public:
   SpellChecker (const ContainerType type);

   // Loads dictionary into memory. Throws exception if any issues 
   void load(const std::string& dictionary); 
 
   // returns true if word is in dictionary else false.
   bool check(const std::string& word) const;

   // adds word to dictionary in-memory
   void add(const std::string& word);

   // Returns number of words in dictionary if loaded else 0 if not yet loaded.
   size_t size(void) const;

   // is string recognized as word to check for spelling or should be skipped
   static bool is_valid(const std::string& word);

private:
   std::unique_ptr<SpellChecker_Impl> impl_;
};

