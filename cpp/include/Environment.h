#pragma once

#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <any>
#include <string>

/**
 * Environment: names storage with lexical scoping.
 * Equivalent to the JavaScript Environment class.
 */
class Environment : public std::enable_shared_from_this<Environment> {
public:
    using Value = std::any;
    using Record = std::unordered_map<std::string, Value>;

    /**
     * Creates environment with given record and optional parent.
     */
    Environment(const Record& record = {}, std::shared_ptr<Environment> parent = nullptr);

    /**
     * Create a shared_ptr to Environment.
     */
    static std::shared_ptr<Environment> create(const Record& record = {}, std::shared_ptr<Environment> parent = nullptr);

    /**
     * Creates a variable with given name and value.
     */
    Value define(const std::string& name, const Value& value);

    /**
     * Returns a value of defined variable, or throws if the variable is not defined.
     */
    Value lookup(const std::string& name);

    /**
     * Updates an existing variable, or throws if the variable is not defined.
     */
    Value assign(const std::string& name, const Value& value);

    /**
     * Returns a specific environment in which a variable is defined, or throws if it's not defined.
     */
    std::shared_ptr<Environment> resolve(const std::string& name);

    /**
     * Get parent environment.
     */
    std::shared_ptr<Environment> getParent() const { return parent_; }

    /**
     * Get record for direct access.
     */
    const Record& getRecord() const { return record_; }

private:
    Record record_;
    std::shared_ptr<Environment> parent_;
};