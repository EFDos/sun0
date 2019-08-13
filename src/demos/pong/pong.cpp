#include <sun.hpp>
#include <graphics.hpp>
#include <resources.hpp>
#include <scene.hpp>

class Pong : public sun::Application
{
public:
    Pong(sun::Context& p_context)
    :   sun::Application(p_context),
        scene_(p_context),
        left_score(0),
        right_score(0)
    {
        window_.set_title("Pong!");

        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));
        renderer_->set_viewport({0, 0, 1280, 720});

        auto res_cache = context_.get_system<sun::ResourceCache>();
        res_cache->set_path("res");

        score_ = scene_.create_entity();
        left_pad_ = scene_.create_entity();
        right_pad_ = scene_.create_entity();
        ball_ = scene_.create_entity();

        score_text_ = score_->create_component<sun::Text>();

        score_text_->set_font(res_cache->get_resource<sun::Font>("Wattauchimma.ttf").get());
        score_text_->set_character_size(72);
        score_text_->set_text("0 | 0");
        score_->set_position(1280 / 2 - score_text_->get_bounding_rect().w / 2, 0);

        left_pad_->set_position(0.f, 360.f - 64.f);
        auto shape = left_pad_->create_component<sun::Shape2D>("shape");
        shape->set_shape(sun::shapes::Rectangle(32.f, 128.f));

        right_pad_->set_position(1280.f - 32.f, 360.f - 64.f);
        shape = right_pad_->create_component<sun::Shape2D>("shape");
        shape->set_shape(sun::shapes::Rectangle(32.f, 128.f));

        ball_->set_position(640.f - 20.f, 360.f - 20.f);
        ball_->set_origin(20.f, 20.f);
        shape = ball_->create_component<sun::Shape2D>("shape");
        shape->set_shape(sun::shapes::Circle(20.f, 120));

        ball_speed_ = {600.f, 0.f};
    }

    void on_update(float delta)
    {
        if (sun::keyboard::is_key_pressed(sun::keyboard::Key::Up)) {
            right_pad_->move(0.f, -180.f * delta);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::Key::Down)) {
            right_pad_->move(0.f, 180.f * delta);
        }

        if (sun::keyboard::is_key_pressed(sun::keyboard::Key::A)) {
            left_pad_->move(0.f, -180.f * delta);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::Key::Z)) {
            left_pad_->move(0.f, 180.f * delta);
        }

        ball_->move(ball_speed_ * delta);

        if (check_collision(ball_->get_component<sun::Shape2D>("shape"),
                            right_pad_->get_component<sun::Shape2D>("shape")))
        {
            ball_speed_.x *= -1;
            ball_speed_.y += ((ball_->get_position().y + 20.f) - (right_pad_->get_position().y + 64.f)) * 12.f;
        }
        if (check_collision(ball_->get_component<sun::Shape2D>("shape"),
                            left_pad_->get_component<sun::Shape2D>("shape")))
        {
            ball_speed_.x *= -1;
            ball_speed_.y += ((ball_->get_position().y + 20.f) - (left_pad_->get_position().y + 64.f)) * 12.f;
        }

        if (ball_->get_position().y + 40.f > 720.f || ball_->get_position().y < 0.f) {
            ball_speed_.y *= -1;
        }
        if (ball_->get_position().x - 40.f > 1280.f) {
            ++left_score;
            respawn();
        }
        if (ball_->get_position().x + 80.f < 0.f) {
            ++right_score;
            respawn();
        }
    }

    void respawn() {
        ball_->set_position(640.f - 20.f, 360.f - 20.f);
        ball_speed_ = {600.f, 0.f};
        score_text_->set_text(std::to_string(left_score) + " | " + std::to_string(right_score));
        score_->set_position(1280 / 2 - score_text_->get_bounding_rect().w / 2, 0);
    }

    bool check_collision(const sun::Shape2D* shape_a, const sun::Shape2D* shape_b)
    {
        auto rect_a = shape_a->get_bounding_rect();
        auto rect_a_pos = shape_a->get_owning_entity()->get_position();
        rect_a.set_point((int)rect_a_pos.x, (int)rect_a_pos.y);
        auto rect_b = shape_b->get_bounding_rect();
        auto rect_b_pos = shape_b->get_owning_entity()->get_position();
        rect_b.set_point((int)rect_b_pos.x, (int)rect_b_pos.y);

        return rect_a.intersects(rect_b) || rect_b.intersects(rect_a);
    }

private:
    sun::SceneTree scene_;

    sun::Vector2f   ball_speed_;

    sun::Entity*    left_pad_;
    sun::Entity*    right_pad_;
    sun::Entity*    ball_;

    sun::Entity*    score_;
    sun::Text*      score_text_;

    int left_score;
    int right_score;
};

SUN_DEFINE_MAIN_APP(Pong);
