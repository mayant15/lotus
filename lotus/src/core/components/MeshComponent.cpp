#include "lotus/scene/components.h"

CMesh::CMesh(const Model& model_, const Shader& shader_)
{
    // TODO: Does this create a new instance? model_ now has two owners?
    model = std::make_shared<Model>(model_);
    shader = std::make_shared<Shader>(shader_);
}
