#include "Layer.h"

namespace Drizzle3D {
    void LayerDispatch::AddLayer(Layer* layer) {
        layers.push_back(layer);
        layer->OnAttach();
    }

    void LayerDispatch::RemoveLayerByName(const std::string& name) {
        layers.erase(std::remove_if(layers.begin(), layers.end(), [&name](const auto& layer) {
            return layer->GetName() == name;
            }), layers.end());
    }

    void LayerDispatch::ShowHideLayerByName(const std::string& name, bool show) {
        auto it = std::find_if(layers.begin(), layers.end(), [&name](const auto& layer) {
            return layer->GetName() == name;
            });

        if (it != layers.end()) {
            (*it)->SetShow(show);
        }
    }

    void LayerDispatch::ChangeLayerOrderByName(const std::string& name, size_t newIndex) {
        auto it = std::find_if(layers.begin(), layers.end(), [&name](const auto& layer) {
            return layer->GetName() == name;
            });

        if (it != layers.end()) {
            size_t currentIndex = std::distance(layers.begin(), it);
            if (currentIndex < newIndex) {
                std::rotate(it, it + 1, layers.begin() + newIndex + 1);
            }
            else if (currentIndex > newIndex) {
                std::rotate(layers.begin() + newIndex, it, it + 1);
            }
        }
    }

    void LayerDispatch::DispatchLayerRender() {
        size_t i = 0;
        while (i < layers.size()) {
            if (true) {
                layers[i]->Render();
            }

            ++i;
        }
    }

    void LayerDispatch::DispatchLayerDetach() {
        for (const auto& layer : layers) {
            layer->OnDetach();
        }
    }
}