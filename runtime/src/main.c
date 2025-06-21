#include "app.h"
#include "event.h"
#include "keyboard.h"
#include "node.h"
#include "nodes/scale.h"
#include "nodes/pos.h"
#include "nodes/rotation.h"
#include "nodes/model.h"
#include "nodes/container.h"
#include "nodes/script.h"

void f(script_t* self, f64 dt) {
    rotation_t* rotation = (rotation_t*)node_get_sibling_by_name((node_t*)self, "rotation");
    rotation->rotation[2] += 10.0f * dt;
}

void on_event(event_type_t type, void* e, void* user_data) {
    if (type == EVENT_TYPE_KEY_PRESSED) {
        if (((key_event_t*)e)->key == PEAR_KEY_ESCAPE) {
            event_send(EVENT_TYPE_QUIT, NULL);
        }
    }
}

int main(int argc, char* argv[]) {
    app_init();
    event_subscribe(on_event, NULL);

    node_t* root = node_root("root");

    container_t* container = node_child(root, container_t, "container");
    pos_t* pos = node_child(container, pos_t, "pos", 2.0f, 0.0f, 0.0f);
    scale_t* avocado_scale = node_child(container, scale_t, "avocado scale", 50.0f, 50.0f, 50.0f);
    model_t* avocado = node_child(container, model_t, "avocado", "assets/Avocado.glb");

    node_child(root, script_t, "script", f);
    node_child(root, rotation_t, "rotation", 0.0f, 0.0f, 0.0f);
    scale_t* scale = node_child(root, scale_t, "scale", 5.0f, 5.0f, 5.0f);
    model_t* model = node_child(root, model_t, "model", "assets/BarramundiFish.glb");

    app_set_root(root);

    app_run();

    return 0;
}
