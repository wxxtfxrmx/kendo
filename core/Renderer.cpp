#include "Renderer.h"

Renderer::Renderer(const PerspectiveCamera &camera) : _camera{camera}{
    GL_CALL(glEnable(GL_DEPTH_TEST));
}

auto Renderer::clear() const -> void {
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

auto Renderer::draw(const Entity &entity, const Shader &shader) const -> void {
    shader.bind();

    const auto& textures = entity.getTextures();
    for (auto i = 0UL; i < textures.size(); i++) {
        const auto& texture = textures[i];
        texture->bind(i);
        shader.setUniform1i(texture->getUniformName(), i);
    }

    entity.getVertexArray().bind();
    entity.getIndexBuffer().bind();

    shader.setUniformMat4f("projection", _camera.projection());
    shader.setUniformMat4f("view", _camera.view());
    shader.setUniformMat4f("model", entity.getModel());

    GL_CALL(glDrawElements(GL_TRIANGLES, entity.getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr));

    entity.getVertexArray().unbind();
    entity.getIndexBuffer().unbind();
    shader.unbind();

    GL_CALL(glActiveTexture(GL_TEXTURE0));
}
