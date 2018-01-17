//
// Created by Richard Hodges on 17/01/2018.
//

#pragma once


#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

// The concept of a polymorphic monster, expressed as a value type
struct Monster
{
    // the concept of a monster's implementation. i.e all monsters
    // will be able to do these operations
    struct ImplementationConcept
    {
        ImplementationConcept()
                : ident_(generate_ident())
        {}

        virtual std::string const& get_name() const = 0;

        virtual std::type_info const& get_type() const = 0;

        virtual bool invoke_hates(Monster const& other) const = 0;

        virtual std::string const& get_type_name() const  = 0;

        // ensure it's polymorphic
        virtual ~ImplementationConcept() = default;

        int unique_ident() const { return ident_; }

    private:

        static int generate_ident() {
            static int next_ident = 0;
            return ++next_ident;
        }

        int ident_;
    };

    using implementation_type = std::unique_ptr<ImplementationConcept>;

private:
    // model the concept for a given monster type
    template<class ActualMonster>
    struct ImplementationModel final : ImplementationConcept
    {
        ImplementationModel(ActualMonster&& m)
                : me_(std::move(m))
        {}

        virtual std::string const& get_name() const override
        {
            return name_of(me_);
        }

        std::type_info const& get_type() const override {
            return typeid(ActualMonster);
        }

        bool invoke_hates(Monster const& other) const override {
            return hates_other(me_, other);
        }

        std::string const& get_type_name() const override {
            return monster_type_name(me_);
        };

        ActualMonster me_;
    };

    template<class ActualMonster>
    static implementation_type make_impl(ActualMonster&& m)
    {
        using monster_type = std::decay_t<ActualMonster>;
        using model_type = ImplementationModel<monster_type>;
        return std::make_unique<model_type>(std::forward<ActualMonster>(m));
    }

public:


    template
            <
                    class Arg,
                    std::enable_if_t<not std::is_same<std::decay_t<Arg>, Monster>::value>* = nullptr
            >
    Monster(Arg&& arg)
            : impl_(make_impl(std::forward<Arg>(arg)))
    {}

    ImplementationConcept& as_concept()
    {
        return *impl_;
    }

    ImplementationConcept const& as_concept() const
    {
        return *impl_;
    }

    bool hates(Monster const& other) const
    {
        return impl_->invoke_hates(other);
    }

    std::string const& type_name() const {
        return impl_->get_type_name();
    }

    std::string const& name() const {
        return impl_->get_name();
    }

private:
    implementation_type impl_;
};




// some utility functions
template<class...Monsters>
void build_party(std::vector<Monster>& party, Monsters&&...monsters)
{
    party.reserve(party.size() + sizeof...(monsters));
    using expand = int[];
    void(expand { 0,
             (party.push_back(std::forward<Monsters>(monsters)), 0)...
    });
}

void handle_encounter(Monster& subject, std::vector<Monster>& the_others);



