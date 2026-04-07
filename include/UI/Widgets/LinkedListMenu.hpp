#pragma once
#include "UI/Widgets/DSAMenuBase.hpp"

namespace UI::Widgets {

    class LinkedListMenu : public DSAMenuBase {
    protected:
        void renderSubMenu(float boxX, float boxY, ActiveMenu type) override;
        std::vector<std::string> getMainButtonLabels() const override;

    public:
        LinkedListMenu(AppContext& context);
    };

}
