#include "integrator.h"
#include "scene.h"
#include "material.h"

class DirectMats : public Integrator
{
public:
    DirectMats(const PropertyList &props) {
        m_samples = props.getInteger("samples", 100);
        m_IS = props.getBoolean("IS", false);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        /* Find the surface that is visible in the requested direction */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            Normal3f normal = hit.normal();
            Point3f pos = ray.at(hit.t());

            const Material* material = hit.shape()->material();
            const LightList &lights = scene->lightList();

            Vector3f tmp_dir, i;
            float pdf;

            if(m_IS)
            {
                for(int i=1; i<=m_samples; i++)
                {
                    tmp_dir = material->is(normal, -ray.direction, pdf);
                    if(normal.dot(tmp_dir) < 0) { tmp_dir=-tmp_dir; }
                    Color3f envmap = scene->backgroundColor(tmp_dir);
                    Color3f BRDF = static_cast<const Ward*>(material)->premultBrdf(-ray.direction, tmp_dir, normal, hit.texcoord());
                    radiance += (envmap * BRDF);
                }
            } else {
                for(int i=1; i<=m_samples; i++)
                {
                    tmp_dir = material->us(normal, i, pdf);
                    Ray reflexion_ray(pos + hit.normal()*Epsilon, tmp_dir);
                    Color3f env_map = scene->backgroundColor(tmp_dir);
                    radiance += env_map * material->brdf(-ray.direction, reflexion_ray.direction, normal, hit.texcoord());
                }
                radiance *= (M_PI/m_samples);
            }
        }else {
            return scene->backgroundColor(ray.direction);
        }

        return radiance / m_samples;
    }

    std::string toString() const {
        return tfm::format("DirectMats[\n"
                           "  m_samples = %d\n"
                           "  m_IS = %d\n"
                           " ]\n",
                           m_samples,
                           m_IS);
    }
private:
    int m_samples;
    bool m_IS;
};

REGISTER_CLASS(DirectMats, "directMats")
