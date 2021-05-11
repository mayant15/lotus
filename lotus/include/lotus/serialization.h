#pragma once

#include <lotus/internal/entt/entity/fwd.hpp>
#include <string>

namespace Lotus
{
    // Tag an entity to be serialized
    struct CSerialize
    {
    };

    struct OutputArchive
    {
        explicit OutputArchive(entt::registry* reg)
                : impl(reg), snapshot(*reg)
        {}

        template<class Component>
        void components()
        {
            std::string name = Component::GetName();
            if (done.find(name) == done.end())
            {
                // If this component hasn't been added to the snapshot yet, add it
                snapshot.template component<Component>(impl);
                done.insert(name);
            }
        }

        void entities()
        {
            snapshot.entities(impl);
        }

        void DumpToFile(const std::string& path)
        {
            impl.dump(path);
        }

    protected:
        entt::snapshot snapshot;
        std::unordered_set<std::string> done {};

        struct impl
        {
        private:
            nlohmann::json data {};
            entt::registry* pReg = nullptr;

        public:
            explicit impl(entt::registry* reg)
                    : pReg(reg)
            {}

            void operator()(entt::entity e)
            {
//                data["entities"].push_back(e);
            }

            void operator()(std::underlying_type<entt::entity>::type size)
            {
//                data["sizes"].push_back(size);
            }

            template<class Component>
            void operator()(entt::entity e, Component comp)
            {
                if (pReg->has<CSerialize>(e))
                {
                    data["components"][std::to_string((entt::id_type) e)][Component::GetName()] = comp;
                }
            }

            void dump(const std::string& path)
            {
                std::ofstream outfile { path };
                outfile << data.dump(2);
            }
        } impl;
    };
}
