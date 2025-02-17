
#ifndef INTERFACE_SCENE_H
#define INTERFACE_SCENE_H

typedef struct Scene
{
    void (*const Draw) (void* self);
    void (*const Update) (void* self);
    bool (*const IsShow) (void* self);
} Scene;

typedef struct Drawable{
    void *self;
    Scene const* scene;
}Drawable;

#define impl_scene(T, name, f_draw, f_update, f_isShow) \
    static Drawable name(T x)                                                                                                 \
    {                                                                                                                  \
        void (*const update_)(T e) = (f_update);                                                                          \
        void (*const draw_)(T e) = (f_draw);                                                                          \
        bool (*const isShow_)(T e) = (f_isShow);                                                                        \
        (void)update_;                                                                                                                                              \
        (void)draw_;                                                                                                   \
        (void)isShow_;                                                                                                   \
        static Scene const scene = {.Update = (void (*const)(void*))(f_update), .Draw = (void (*const)(void*))(f_draw), .IsShow = (bool (*const)(void*))(f_isShow) };                                             \
        return (Drawable){.scene = &scene, .self = x};                                                                       \
    }

#endif // INTERFACE_SCENE_H