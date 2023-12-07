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

    void LayerDispatch::PushFront(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end()) {
            Layer* layerCopy = new Layer(*(*it)); // Create a copy of the layer
            layers.erase(it); // Remove the original layer
            layers.insert(layers.begin(), layerCopy); // Add the copy to the front
            layerCopy->OnAttach();

            // Iterate through the remaining layers and add them back
            for (const auto& layer : layers) {
                if (layer->GetName() != layerName) {
                    AddLayer(layer); // Assuming AddLayer adds the layer and calls OnAttach
                }
            }
        }
    }

    void LayerDispatch::PushBack(const std::string& layerName) {
        auto it = std::find_if(layers.begin(), layers.end(), [&layerName](const auto& layer) {
            return layer->GetName() == layerName;
            });

        if (it != layers.end()) {
            Layer* layerCopy = new Layer(*(*it)); // Create a copy of the layer
            layers.erase(it); // Remove the original layer
            layers.push_back(layerCopy); // Add the copy to the back
            layerCopy->OnAttach();

            // Iterate through the remaining layers and add them back
            for (const auto& layer : layers) {
                if (layer->GetName() != layerName) {
                    AddLayer(layer); // Assuming AddLayer adds the layer and calls OnAttach
                }
            }
        }
    }




    void LayerDispatch::PushBackward(const std::string& name) {
        auto it = std::find_if(layers.begin(), layers.end(), [&name](const auto& layer) {
            return layer->GetName() == name;
            });

        if (it != layers.end() && it != layers.begin()) {
            std::iter_swap(it, it - 1);
        }
    }

    void LayerDispatch::PushForward(const std::string& name) {
        auto it = std::find_if(layers.begin(), layers.end(), [&name](const auto& layer) {
            return layer->GetName() == name;
            });

        if (it != layers.end() && it + 1 != layers.end()) {
            std::iter_swap(it, it + 1);
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