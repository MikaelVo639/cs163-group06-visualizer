#pragma once
#include "UI/Components/DSAMenuBase.hpp"

namespace UI::Widgets {

    class HeapMenu : public DSAMenuBase {
    protected:
        void renderSubMenu(float boxX, float boxY, ActiveMenu type) override;
        std::vector<std::string> getMainButtonLabels() const override;

    public:
        HeapMenu(AppContext& context);
    };

}