#include <sun.hpp>

class pong : public sun::application
{
public:
    pong(sun::context& p_context)
    :   sun::application(p_context),
        scene_(p_context),
        left_score(0),
        right_score(0)
    {
        window_.set_title("Pong!");

        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        renderer_->set_viewport({0, 0, 1280, 720});
        auto res_cache = context_.get_system<sun::resource_cache>();
        res_cache->set_path("res");

        auto score_entity = scene_.create_entity();
        left_pad_ = scene_.create_entity();
        right_pad_ = scene_.create_entity();
        ball_ = scene_.create_entity();

        score_entity->move(640.f - 64.f, 0.f);
        score_text_ = score_entity->create_component<sun::text>();

        score_text_->set_font(res_cache->get_resource<sun::font>("Wattauchimma.ttf").get());
        score_text_->set_character_size(72);
        score_text_->set_text("0 | 0");

        left_pad_->set_position(0.f, 360.f - 64.f);
        auto shape = left_pad_->create_component<sun::shape2D>("shape");
        shape->set_shape(sun::shapes::rectangle(32.f, 128.f));

        right_pad_->set_position(1280.f - 32.f, 360.f - 64.f);
        shape = right_pad_->create_component<sun::shape2D>("shape");
        shape->set_shape(sun::shapes::rectangle(32.f, 128.f));

        ball_->set_position(640.f - 20.f, 360.f - 20.f);
        shape = ball_->create_component<sun::shape2D>("shape");
        shape->set_shape(sun::shapes::circle(20.f, 120));

        ball_speed_ = {8.f, 0.f};
    }

    void on_update()
    {
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::UP)) {
            right_pad_->move(0.f, -8.f);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::DOWN)) {
            right_pad_->move(0.f, 8.f);
        }

        if (sun::keyboard::is_key_pressed(sun::keyboard::key::A)) {
            left_pad_->move(0.f, -8.f);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::Z)) {
            left_pad_->move(0.f, 8.f);
        }

        ball_->move(ball_speed_);

        if (check_collision(ball_->get_component<sun::shape2D>("shape"),
                            right_pad_->get_component<sun::shape2D>("shape")))
        {
            ball_speed_.x *= -1;
            ball_speed_.y += ((ball_->get_position().y + 20.f) - (right_pad_->get_position().y + 64.f)) * 0.15f;
        }
        if (check_collision(ball_->get_component<sun::shape2D>("shape"),
                            left_pad_->get_component<sun::shape2D>("shape")))
        {
            ball_speed_.x *= -1;
            ball_speed_.y += ((ball_->get_position().y + 20.f) - (left_pad_->get_position().y + 64.f)) * 0.15f;
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
        ball_speed_ = {8.f, 0.f};
        score_text_->set_text(std::to_string(left_score) + "|" + std::to_string(right_score));
    }

    bool check_collision(const sun::shape2D* shape_a, const sun::shape2D* shape_b)
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
    sun::scene_tree scene_;

    sun::vector2f   ball_speed_;

    sun::entity*    left_pad_;
    sun::entity*    right_pad_;
    sun::entity*    ball_;

    sun::text*      score_text_;

    int left_score;
    int right_score;
};

SUN_DEFINE_MAIN_APP(pong);
