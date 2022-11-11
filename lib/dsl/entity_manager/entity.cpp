#include "entity_manager/entity.h"

namespace dsl
{
    namespace entity_manager
    {
        // Methods for EntityEvent
        EntityEvent::EntityEvent(const Entity &e) : entity(e)
        {
        }

        // Public methods for Entity
        Entity::Entity(value_type value) : _value(value) {}

        Entity::Entity() {}

        bool Entity::subscribe(const std::string &name, const Subscription &subscription)
        {
            const auto [it, success] = __subscriptions.insert({name, subscription});
            return success;
        }

        bool Entity::unsubscribe(const std::string &name)
        {
            return __subscriptions.erase(name) != 0;
        }

        void Entity::unsubscribe_all()
        {
            __subscriptions.clear();
        }

        void Entity::publish(bool value_changed) const
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

        Entity &Entity::set(value_type value)
        {
            bool changed = _value != value;
            _value = value;
            publish(changed);
            return *this;
        }

        // Methods to get the value and the valuetype
        value_type Entity::get() const
        {
            return _value;
        }

        entity_type_name Entity::get_type() const
        {
            return entity_type_name(_value.index());
        }

        Entity &Entity::operator=(value_type value)
        {
            return set(value);
        }

        Entity &Entity::operator++(int)
        {
            return ++(*this);
        }

        Entity &Entity::operator++()
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

        Entity &Entity::operator--(int)
        {
            return --(*this);
        }

        Entity &Entity::operator--()
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

        bool Entity::operator!()
        {
            if (get_type() == Bool)
            {
                return !std::get<bool>(_value);
            }
            // TODO: Exception
            return true;
        }

        Entity::operator bool() const
        {
            if (get_type() == Bool)
            {
                return std::get<bool>(_value);
            }
            // TODO: Exception
            return false;
        }

        Entity::operator int() const
        {
            if (get_type() == Integer)
            {
                return std::get<int>(_value);
            }
            // TODO: Exception
            return 0;
        }

        Entity::operator double() const
        {
            if (get_type() == Double)
            {
                return std::get<double>(_value);
            }
            // TODO: Exception
            return 0.d;
        }

        Entity::operator std::string() const
        {
            if (get_type() == String)
            {
                return std::get<std::string>(_value);
            }
            // TODO: Exception
            return std::string("");
        }

        // Global objects
        std::unordered_map<std::string, Entity> entities;
    };
};