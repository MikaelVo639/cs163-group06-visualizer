#pragma once
#include "UI/Components/DSAMenuBase.hpp"

namespace UI::Widgets {

class HeapMenu : public DSAMenuBase {
protected:
    void renderSubMenu(float boxX, float boxY, ActiveMenu type) override;
    std::vector<std::string> getMainButtonLabels() const override;

public:
    explicit HeapMenu(AppContext& context);
};

} // namespace UI::Widgets