#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "matrix.h"

#include <SFML/Graphics/Color.hpp>

/**
 * This class contains every model which is to be rendered on the scene
 * it will contain the mesh, along with other properties like 
 * model matrix, textures and material files.
 * */

struct Texture {
    unsigned char* image;
    int width;
    int height;
    int channels;

    Texture() {
        image = nullptr;
    }

    ~Texture() {
        delete [] image;
    }

    sf::Color getColor(int i, int j) {
        if (!image) return sf::Color(0, 0, 0, 0xff);
        unsigned char* pixelOffset = image + (i + j * width) * channels;
        return sf::Color(pixelOffset[0], pixelOffset[1], pixelOffset[2], 0xff);
    }

    bool load_texture(std::string filename);
};

class Model {
private:
    Mesh* m_Mesh;
    Matrix4f m_ModelMatrix;
    std::string m_filepath;
    Texture* m_Diffuse;
    
public:
    Model(std::string path);
    Model();
    ~Model();
    void transformModel(Vector3f translate, Vector3f rotate, Vector3f scale);
    void loadDiffuse(std::string filename);
    const Matrix4f& getModelMatrix() const;
    Texture* getDiffuse();
    Mesh* getMesh();
};

#endif      // MODEL_H
