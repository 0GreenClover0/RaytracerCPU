#include "Raytracer.h"

#include "AK/MathUtils.h"
#include "AK/Vec3.h"
#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"

void Raytracer::run()
{
    // World
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f));
    world.add(std::static_pointer_cast<Hittable>(std::make_shared<Sphere>(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = random_float();
            Vec3 center(static_cast<float>(a) + 0.9f * random_float(), 0.2f, static_cast<float>(b) + 0.9f * random_float());

            if ((center - Vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8f)
                {
                    // Diffuse
                    Vec3 albedo = Vec3::random() * Vec3::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95f)
                {
                    // Metal
                    Vec3 albedo = Vec3::random(0.5f, 1.0f);
                    float fuzz = random_float(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                } else
                {
                    // Glass
                    sphere_material = std::make_shared<Dielectric>(1.5f);
                }

                world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    world.add(std::make_shared<Sphere>(Vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(Vec3(0.4f, 0.2f, 0.1f));
    world.add(std::make_shared<Sphere>(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = std::make_shared<Metal>(Vec3(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(std::make_shared<Sphere>(Vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));

    Camera camera(16.0f / 9.0f, 100, 100, 50);
    camera.vertical_fov = 20.0f;
    camera.look_from = Vec3(13.0f, 2.0f, 3.0f);
    camera.look_at = Vec3(0.0f, 0.0f, 0.0f);
    camera.v_up = Vec3(0.0f, 1.0f, 0.0f);

    camera.render(world);
}
