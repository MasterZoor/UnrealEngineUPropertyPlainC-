#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

// Reflection system

struct PropertyInfo {
    std::string Name;
    std::string Type;
    std::string Meta;
    void* Ptr;
};

struct PropertyRegistry {
    std::vector<PropertyInfo> Properties;

    void Add(const std::string& name, const std::string& type, const std::string& meta, void* ptr) {
        Properties.push_back({ name, type, meta, ptr });
    }

    void PrintAll() const {
        for (const auto& p : Properties) {
            std::cout << "Property: " << p.Name
                << " | Type: " << p.Type
                << " | Meta: " << p.Meta;

            if (p.Type == typeid(int).name()) {
                std::cout << " | Value: " << *static_cast<int*>(p.Ptr);
            }
            else if (p.Type == typeid(float).name()) {
                std::cout << " | Value: " << *static_cast<float*>(p.Ptr);
            }
            else if (p.Type == typeid(std::string).name()) {
                std::cout << " | Value: " << *static_cast<std::string*>(p.Ptr);
            }

            std::cout << "\n";
        }
    }
};


// CRTP base class
template <typename Derived>
class Reflectable {
public:
    static PropertyRegistry& Registry() {
        static PropertyRegistry reg;
        return reg;
    }

    PropertyRegistry& GetRegistry() const {
        return Registry();
    }
};

// UE-style macros
// Macro above variable declaration
#define MYPROPERTY() /* nothing, just marker */
#define MYPROPERTY_META(meta) /* nothing, just marker */


// Example class
class Player : public Reflectable<Player> {
public:
    using DerivedPlaceholder = Player;
    // UE-style properties
    MYPROPERTY()
    int Health = 100;

    MYPROPERTY_META(EditAnywhere)
    std::string Name = "Zoran";

    MYPROPERTY()
    float Speed = 2.0;

    // Constructor registers properties automatically
    Player() {
        GetRegistry().Add("Health", typeid(Health).name(), "VisibleAnywhere", &Health);
        GetRegistry().Add("Name", typeid(Name).name(), "EditAnywhere", &Name);
        GetRegistry().Add("Speed", typeid(Speed).name(), "VisibleAnywhere", &Speed);
    }
};

// Main
int main() {
    Player p;

    std::cout << "=== Initial ===\n";
    p.GetRegistry().PrintAll();

    // Modify values
    p.Health = 250;
    p.Name = "Neo";
    p.Speed = 5.0;
    std::cout << "\n=== After Changes ===\n";
    p.GetRegistry().PrintAll();

    std::cout << "\nDirect access: Health=" << p.Health << ", Name=" << p.Name << " ,Speed= " << p.Speed<< "\n";

    return 0;
}

