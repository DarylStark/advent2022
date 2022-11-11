#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <thread>

namespace dsl
{
    namespace entity_manager
    {
        // Typedef for the variant that we are going to use in the Entity
        typedef std::variant<bool, int, double, std::string>
            value_type;

        // Enum with the types; should match the order of the `variant` in the line
        // above this.
        enum entity_type_name
        {
            Bool,
            Integer,
            Double,
            String
        };

        // We declare the Entity class early becasue it is needed by the EntityEvent
        // data structure
        class Entity;

        // The EntityEvent class can be used by callbacks to identify the event that
        // happend.
        class EntityEvent
        {
        public:
            const Entity &entity;
            bool changed;
            std::string identifier = "";

            EntityEvent(const Entity &e);
        };

        // Class to define a subscription
        struct Subscription
        {
            std::function<void(const EntityEvent &)> function;
            bool on_unchanged = false;
            bool threaded = false;
            std::string identifier = "";
        };

        // The Entity class defines entities in a Observer-pattern
        class Entity
        {
        private:
            std::unordered_map<std::string, const Subscription> __subscriptions;

        protected:
            value_type _value;

        public:
            // Constructors
            Entity(value_type value);
            Entity();

            // We don't need a copy constructor
            Entity(const Entity &) = delete;

            // Methods to subscribe and unsubscribe
            bool subscribe(const std::string &name, const Subscription &subscription);
            bool unsubscribe(const std::string &name);
            void unsubscribe_all();

            // Methods to publish to the subscribers
            void publish(bool value_changed) const;

            // Method to set the value
            Entity &set(value_type value);

            // Methods to get the value and the valuetype
            value_type get() const;
            entity_type_name get_type() const;

            // Overloading operators
            Entity &operator=(value_type value);
            Entity &operator++(int);
            Entity &operator++();
            Entity &operator--(int);
            Entity &operator--();
            bool operator!();

            // Operator for type casting
            operator bool() const;
            operator int() const;
            operator double() const;
            operator std::string() const;
        };

        // Export a global object for entities
        extern std::unordered_map<std::string, Entity> entities;
    };
};

#endif // __ENTITY_H__