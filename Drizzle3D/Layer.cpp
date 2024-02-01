/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
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
            if (layers[i]->IsShown()) {
                layers[i]->Render();
            }
            i++;
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

    void LayerDispatch::PushFront(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end() && it != layers.begin()) {
            std::iter_swap(it, layers.begin());
        }
    }

    void LayerDispatch::PushBack(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end() && it + 1 != layers.end()) {
            std::iter_swap(it, layers.end() - 1);
        }
    }

    void LayerDispatch::PushForward(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end() && std::next(it) != layers.end()) {
            std::iter_swap(it, std::next(it));
        }
    }

    void LayerDispatch::PushBackward(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end() && it != layers.begin()) {
            std::iter_swap(it, std::prev(it));
        }
    }
}