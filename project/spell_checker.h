#pragma once

#include <memory>
#include <string>

enum class ContainerType { Vector, Set, Unordered_Set };

// exception on failure to load dictionary file
class SpellChecker_InvalidDictFile {} ;

//class SpellChecker_Impl
class SpellChecker_Impl
{
public:
	virtual void load (const std::string &) = 0;
	virtual bool check (const std::string &) const = 0;
	virtual void add (const std::string &) = 0;
	virtual size_t size(void) const = 0;
	virtual ~SpellChecker_Impl() {}
};

class SpellChecker
{
public:
	SpellChecker (const ContainerType);

	// Loads dictionary into memory. Throws exception if any issues 
	void load(const std::string &); 
 
	// returns true if word is in dictionary else false.
	bool check(const std::string &) const;

	// adds word to dictionary in-memory
	void add(const std::string &);

	// Returns number of words in dictionary if loaded else 0 if not yet loaded.
	size_t size(void) const;

	// is string recognized as word to check for spelling or should be skipped
	static bool is_valid(const std::string &);

private:
	std::unique_ptr<SpellChecker_Impl> impl_;
};