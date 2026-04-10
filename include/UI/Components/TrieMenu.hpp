#pragma once
#include "UI/Components/DSAMenuBase.hpp"

namespace UI::Widgets {

    class TrieMenu : public DSAMenuBase {
    protected:
        // These stay the same because we are overriding the base class behavior
        void renderSubMenu(float boxX, float boxY, ActiveMenu type) override;
        std::vector<std::string> getMainButtonLabels() const override;

    public:
        explicit TrieMenu(AppContext& context);
    };

}