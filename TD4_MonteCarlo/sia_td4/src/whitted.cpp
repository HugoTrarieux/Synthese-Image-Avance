#include "integrator.h"
#include "scene.h"
#include "material.h"
#include "areaLight.h"

class Whitted : public Integrator
{
public:
    Whitted(const PropertyList &props) {
        m_maxRecursion = props.getInteger("maxRecursion",4);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        // stopping criteria:
        if(ray.recursionLevel>=m_maxRecursion) {
            return radiance;
        }

        /* Find the surface that is visible in the requested direction */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            Normal3f normal = hit.normal();
            Point3f pos = ray.at(hit.t());

            const Material* material = hit.shape()->material();

            const LightList &lights = scene->lightList();
            for(LightList::const_iterator it=lights.begin(); it!=lights.end(); ++it)
            {
                Vector3f lightDir;
                float dist;
                Color3f intensity;

                if(dynamic_cast<const AreaLight*>(*it)) { 
                    const AreaLight* light = dynamic_cast<const AreaLight*>(*it); 
                    // source étendue 
                    Vector2f sourceSize = light->size();
                    float u = Eigen::internal::random<float>(sourceSize(0)/-2, sourceSize(0)/2);
                    float v = Eigen::internal::random<float>(sourceSize(1)/-2, sourceSize(1)/2);
                    Point3f randomPointInArea = light->position() + light->uVec()*u + light->vVec()*v;

                    lightDir = randomPointInArea - pos;
                    dist = lightDir.norm();
                    lightDir = lightDir.normalized();
                    intensity = light->intensity(pos, randomPointInArea);
                } else { 
                    // lampe ponctuelle ou directionnelle 
                    lightDir = (*it)->direction(pos, &dist);
                    intensity = (*it)->intensity(pos);
                }

                Ray shadow_ray(pos + normal*Epsilon, lightDir, true);
                Hit shadow_hit;
                scene->intersect(shadow_ray, shadow_hit);

                Color3f attenuation = Color3f(1.f);
                if(shadow_hit.t()<dist){
                    if(!shadow_hit.shape()->isEmissive())
                        attenuation = 0.5f * shadow_hit.shape()->material()->transmissivness();
                    if((attenuation <= 1e-6).all())
                        continue;
                }
                float cos_term = std::max(0.f,lightDir.dot(normal));
                Color3f brdf = material->brdf(-ray.direction, lightDir, normal, hit.texcoord());
                radiance += intensity * cos_term * brdf * attenuation;
            }

            // reflexions
            if((material->reflectivity() > 1e-6).any())
            {
                Vector3f r = (ray.direction - 2.*ray.direction.dot(hit.normal())*hit.normal()).normalized();
                Ray reflexion_ray(pos + hit.normal()*Epsilon, r);
                reflexion_ray.recursionLevel = ray.recursionLevel + 1;
                float cos_term = std::max(0.f,r.dot(normal));
                radiance += material->reflectivity() * Li(scene, reflexion_ray) * cos_term;
            }

            // refraction
            if((material->transmissivness() > 1e-6).any())
            {
                float etaA = material->etaA(), etaB = material->etaB();
                bool entering = -normal.dot(ray.direction) > 0;
                if(!entering) {
                    std::swap(etaA,etaB);
                    normal = -normal;
                }
                Vector3f r;
                if(refract(normal,ray.direction,etaA,etaB,r)) {
                    Ray refraction_ray(pos - normal*Epsilon, r);
                    refraction_ray.recursionLevel = ray.recursionLevel + 1;
                    float cos_term = std::max(0.f,-r.dot(normal));
                    radiance += material->transmissivness() * Li(scene, refraction_ray) * cos_term;
                }
            }

        }
        
        else if(ray.recursionLevel == 0)
            return scene->backgroundColor();

        return radiance;
    }

    std::string toString() const {
        return tfm::format("Whitted[\n"
                           "  max recursion = %f\n"
                           " ]\n",
                           m_maxRecursion);
    }
private:
    int m_maxRecursion;
};

REGISTER_CLASS(Whitted, "whitted")
