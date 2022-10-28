#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <thread>
#include <exception>

namespace nemo
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

        EntityEvent(const Entity &e) : entity(e) {}
    };

    // Class to define a subscription
    class Subscription
    {
    public:
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
        Entity(value_type value) : _value(value) {}
        Entity() {}

        // We don't need a copy constructor
        Entity(const Entity &) = delete;

        // Methods to subscribe and unsubscribe
        bool subscribe(const std::string &name, const Subscription &subscription)
        {
            const auto [it, success] = __subscriptions.insert({name, subscription});
            return success;
        }

        bool unsubscribe(const std::string &name)
        {
            return __subscriptions.erase(name) != 0;
        }

        // Methods to publish to the subscribers
        void publish(bool value_changed) const
        {
            for (const auto &[name, sub] : __subscriptions)
            {
                if ((sub.on_unchanged && !value_changed) || (value_changed))
                {
                    EntityEvent e(*this);
                    e.changed = value_changed;
                    e.identifier = sub.identifier;
                    if (sub.threaded)
                    {
                        // Start the function in it's own thread
                        std::thread thread(sub.function, e);
                        thread.detach();
                    }
                    else
                    {
                        // Start the function in the current thread
                        sub.function(e);
                    }
                }
            }
        }

        // Method to set the value
        Entity &set(value_type value)
        {
            bool changed = _value != value;
            _value = value;
            publish(changed);
            return *this;
        }

        // Methods to get the value and the valuetype
        value_type get() const { return _value; }
        entity_type_name get_type() const { return entity_type_name(_value.index()); }

        // Overloading operators
        Entity &operator=(value_type value)
        {
            return set(value);
        }

        Entity &operator++(int)
        {
            return ++(*this);
        }

        Entity &operator++()
        {
            // Increment operator is only available for integers and doubles
            if (get_type() == Integer)
            {
                return *this = std::get<int>(_value) + 1;
            }
            else if (get_type() == Double)
            {
                return *this = std::get<double>(_value) + 1;
            }
            return *this;
        }

        Entity &operator--(int)
        {
            return --(*this);
        }

        Entity &operator--()
        {
            // Decrement operator is only available for integers and doubles
            if (get_type() == Integer)
            {
                return *this = std::get<int>(_value) - 1;
            }
            else if (get_type() == Double)
            {
                return *this = std::get<double>(_value) - 1;
            }
            return *this;
        }

        bool operator!()
        {
            if (get_type() == Bool)
            {
                return !std::get<bool>(_value);
            }
            // TODO: Exception
            return true;
        }

        // Operator for type casting
        operator bool() const
        {
            if (get_type() == Bool)
            {
                return std::get<bool>(_value);
            }
            // TODO: Exception
            return false;
        }

        operator int() const
        {
            if (get_type() == Integer)
            {
                return std::get<int>(_value);
            }
            // TODO: Exception
            return 0;
        }

        operator double() const
        {
            if (get_type() == Double)
            {
                return std::get<double>(_value);
            }
            // TODO: Exception
            return 0.d;
        }

        operator std::string() const
        {
            if (get_type() == String)
            {
                return std::get<std::string>(_value);
            }
            // TODO: Exception
            return std::string("");
        }
    };
};

#endif // __ENTITY_H__