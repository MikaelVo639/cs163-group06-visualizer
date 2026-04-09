#include "States/DSAScreenBase.hpp"

DSAScreenBase::DSAScreenBase(AppContext& context) 
    : ctx(context), myGraph(context, true) 
{
    graphView = ctx.window.getDefaultView();
}


void DSAScreenBase::handleEvent(const sf::Event& event) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(ctx.window);
    sf::Vector2f worldPos = ctx.window.mapPixelToCoords(pixelPos, graphView);
    myGraph.handleEvent(event, worldPos);

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Right) {
            isPanning = true;
            lastPanMousePos = mousePressed->position; 
        }
    }
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Right) {
            isPanning = false;
        }
    }
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (isPanning) {
            sf::Vector2i currentMousePos = mouseMoved->position;
            
            sf::Vector2f delta = ctx.window.mapPixelToCoords(lastPanMousePos, graphView) - 
                                    ctx.window.mapPixelToCoords(currentMousePos, graphView);
            
            graphView.move(delta);
            lastPanMousePos = currentMousePos;
        }
    }
    
    else if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (wheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
            float zoomAmount = 1.1f;
            if (wheelScrolled->delta > 0) {
                graphView.zoom(1.0f / zoomAmount); 
            } else if (wheelScrolled->delta < 0) {
                graphView.zoom(zoomAmount);        
            }
        }
    }
}

void DSAScreenBase::update() {
    // 1. get the pixel coords of the screen
    sf::Vector2i pixelPos = sf::Mouse::getPosition(ctx.window);
    
    // 2. use the camera to convert to riel coords
    sf::Vector2f worldPos = ctx.window.mapPixelToCoords(pixelPos, graphView);
    
    // 3. Graph manage the hover with the riel coords
    myGraph.update(worldPos); 
}

void DSAScreenBase::draw(){
    ctx.window.setView(graphView);
    myGraph.draw();
    
    ctx.window.setView(ctx.window.getDefaultView());
}