#include <sun.hpp>

class Sandbox : public sun::Application
{
public:

	Sandbox(sun::Context& p_context)
	:   sun::Application(p_context),
	    scene_(p_context)
	{
        renderer_->set_color(sun::Color::DARK_GREY);
        renderer_->set_viewport({0, 0, 1280, 720});
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));

        context_.get_system<sun::PhysicsServer>()->set_debug_draw(true);

        auto res_cache = context_.get_system<sun::ResourceCache>();
        res_cache->set_path("res");
        auto tex = res_cache->get_resource<sun::Texture>("bototem.png");

        auto ground = scene_.create_entity();
        entity_ = scene_.create_entity();

        ground->set_name("Ground");
        ground->set_position(0, 600);
        auto ground_body = ground->create_component<sun::RigidBody>();

        ground_body->create(sun::shapes::Rectangle(1280, 32),
            sun::RigidBody::Type::Static);

        entity_->set_name("Jack");
        auto sprite = entity_->create_component<sun::Sprite>("sprite");
        auto ent_body = entity_->create_component<sun::RigidBody>("body");
        auto camera = entity_->create_component<sun::Camera>();
        auto script = entity_->create_component<sun::Script>();
        auto animation = entity_->create_component<sun::Animation>("anim");

        auto& track = animation->create_track(*sprite, "frame", sun::Time::seconds(3.f));

        track.insert_key({0, sun::Time::seconds(0.f)});
        track.insert_key({1, sun::Time::seconds(3.f)});

        script->load("res/test.lua");

        camera->set_follow(true);
        camera->set_follow_speed(10.f);

        ent_body->create(sun::shapes::Convex({{-32, 18}, {0, -36}, {32, 18}}),
            sun::RigidBody::Type::Dynamic);
        sprite->set_texture(tex.get());
        sprite->set_frames(2, 1);
        entity_->set_origin(48, 56);
        entity_->set_scale(0.75f, 0.75f);
	}

    ~Sandbox() {
    }

	void on_event(sun::Event& e) override {
        sun::Application::on_event(e);
	}

    void on_update(float delta) override {
        //sun_logf("Delta time: %f", delta);
    }

private:

    sun::SceneTree scene_;
    sun::Entity* entity_;
};

SUN_DEFINE_MAIN_APP(Sandbox)
