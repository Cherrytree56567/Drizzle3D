#include "Layer.h"

namespace Drizzle3D {
    void LayerDispatch::AddLayer(Layer* layer) {
        layers.push_back(layer);
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

    void LayerDispatch::DispatchLayerAttach() {
        for (const auto& layer : layers) {
            layer->OnAttach();
        }
    }
}