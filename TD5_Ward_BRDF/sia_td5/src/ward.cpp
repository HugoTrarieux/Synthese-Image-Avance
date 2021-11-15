#include "material.h"

Ward::Ward(const PropertyList &propList)
    : Diffuse(propList.getColor("diffuse",Color3f(0.2)))
{
    m_reflectivity = propList.getColor("reflectivity",Color3f(0.0));
    m_transmissivness = propList.getColor("transmissivness",Color3f(0.0));
    m_etaA = propList.getFloat("etaA",1);
    m_etaB = propList.getFloat("etaB",1);
    m_specularColor = propList.getColor("specular",Color3f(0.9));
    m_diffuseColor = propList.getColor("diffuse",Color3f(0.2));
    m_alphaX = propList.getFloat("alphaX",0.2);
    m_alphaY = propList.getFloat("alphaY",0.2);

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Ward::brdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{
    Vector3f i = {0, 1, 0};
    Vector3f b2 = i.cross(normal);

    Vector3f T = normal.cross(b2).normalized();
    Vector3f B = T.cross(normal).normalized();

    Vector3f h;
    float norm = (viewDir+lightDir).norm();
    if(norm<0.0001){
        h = normal;
    } else{
        h = ((viewDir+lightDir)/norm).normalized();
    }

    //isotrope
    //float expo = exp(-( (1-std::max(h.dot(normal),0.0f)) / (std::max(h.dot(normal),0.0f) * m_alphaX*m_alphaY)) );
    //anisotrope
    float expo = exp(-( ((h.dot(T)/m_alphaX)*(h.dot(T)/m_alphaX) + (h.dot(B)/m_alphaY)*(h.dot(B)/m_alphaY)) / (h.dot(normal))*(h.dot(normal)) ));

    Color3f color = m_diffuseColor/M_PI + (m_specularColor / (4.0f*M_PI*m_alphaX*m_alphaY*sqrt( std::max(lightDir.dot(normal),0.0001f) * std::max(viewDir.dot(normal),0.0001f) ) )) * expo;
    return color;
}

Color3f Ward::premultBrdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& n, const Vector2f& uv) const
{
    if (lightDir.dot(n) * viewDir.dot(n) <= 0) { 
        return Color3f(0,0,0);
    }

    Vector3f h = (viewDir + lightDir).normalized();
    return m_specularColor * h.dot(lightDir) * h.dot(n) * h.dot(n) * h.dot(n) * sqrt(viewDir.dot(n) / lightDir.dot(n));
}

Vector3f Ward::is(const Normal3f& n, const Vector3f &i, float& pdf) const
{
    float u = (float)rand() / RAND_MAX;
    float v = (float)rand() / RAND_MAX;
    float phiH = atan( (m_alphaY/m_alphaX) * tan(2*M_PI*v));

    float squared_cos = (cos(phiH)*cos(phiH)) / (m_alphaX*m_alphaX);
    float squared_sin = (sin(phiH)*sin(phiH)) / (m_alphaY*m_alphaY);
    float thetaH = atan(sqrt(-log(u) / (squared_cos+squared_sin)));

    Vector3f h = {sin(thetaH)*cos(phiH), sin(thetaH)*sin(phiH), cos(thetaH)};

    Vector3f d = {0,1,0};
    Vector3f x = (d - (d.dot(n) * n)).normalized();
    Vector3f y = x.cross(n);
    h = h.x() * x + h.y() * y + h.z() * n;

    Vector3f o = 2.0f * std::max(i.dot(h), 0.0001f) * h - i;

    return o;
}

std::string Ward::toString() const {
    return tfm::format(
        "Ward [\n" 
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  alphaX = %f  alphaY = %f\n"
        "]", m_diffuseColor.toString(),
             m_specularColor.toString(),
             m_alphaX, m_alphaY);
}

REGISTER_CLASS(Ward, "ward")
