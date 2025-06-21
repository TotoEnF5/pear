#include "app.h"
#include "node.h"
#include "nodes/scale.h"
#include "nodes/pos.h"
#include "nodes/model.h"
#include "nodes/container.h"

int main(int argc, char* argv[]) {
    app_init();

    node_t* root = node_root("root");

    container_t* container = node_child(root, container_t, "container");
    pos_t* pos = node_child(container, pos_t, "pos", 2.0f, 0.0f, 0.0f);
    scale_t* avocado_scale = node_child(container, scale_t, "avocado scale", 50.0f, 50.0f, 50.0f);
    model_t* avocado = node_child(container, model_t, "avocado", "assets/Avocado.glb");

    scale_t* scale = node_child(root, scale_t, "scale", 5.0f, 5.0f, 5.0f);
    model_t* model = node_child(root, model_t, "model", "assets/BarramundiFish.glb");

    app_set_root(root);

    app_run();

    node_delete(root);

    return 0;
}
