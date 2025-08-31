#include "Environment.h"

std::shared_ptr<Environment> Environment::create(const Record& record, std::shared_ptr<Environment> parent) {
    return std::shared_ptr<Environment>(new Environment(record, parent));
}

Environment::Environment(const Record& record, std::shared_ptr<Environment> parent)
    : record_(record), parent_(parent) {
}

Environment::Value Environment::define(const std::string& name, const Value& value) {
    record_[name] = value;
    return value;
}

Environment::Value Environment::lookup(const std::string& name) {
    auto env = resolve(name);
    return env->record_[name];
}

Environment::Value Environment::assign(const std::string& name, const Value& value) {
    auto env = resolve(name);
    env->record_[name] = value;
    return value;
}

std::shared_ptr<Environment> Environment::resolve(const std::string& name) {
    if (record_.find(name) != record_.end()) {
        return shared_from_this();
    }

    if (parent_ == nullptr) {
        throw std::runtime_error("Variable \"" + name + "\" is not defined.");
    }

    return parent_->resolve(name);
}