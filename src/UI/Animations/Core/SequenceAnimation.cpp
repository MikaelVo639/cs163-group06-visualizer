#include "UI/Animations/Core/SequenceAnimation.hpp"

namespace UI::Animations {

    void SequenceAnimation::add(std::unique_ptr<AnimationBase> anim) {
        if (anim) {
            sequence.push(std::move(anim));
        }
    }

    void SequenceAnimation::update(float dt) {
        while (!sequence.empty() && dt > 0.f) {
            sequence.front()->update(dt);
            if (sequence.front()->isFinished()) {
                sequence.pop();
            } 
            else {
                break;
            }
        }
    }

    bool SequenceAnimation::isFinished() const {
        return sequence.empty();
    }

} // namespace UI::Animations