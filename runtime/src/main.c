#include "app.h"
#include "nodes/root.h"
#include "nodes/transform.h"
#include "log.h"

void f(node_t* node) {
    PEAR_INFO("%s", node->name);
}

int main(int argc, char* argv[]) {
    root_t* root = root_new("root");
    transform_t* transform = node_child(root, transform_t, "transform1");
    transform_t* t2 = node_child(transform, transform_t, "transform2");
    node_map((node_t*)root, f);
    node_delete(transform, transform_t);
    node_map((node_t*)root, f);
    root_delete(root);

    app_init();
    app_run();

    return 0;
}
