#include <sun.hpp>

    /*
     * DEPRECATED
     */

class pong : public sun::application
{
public:
    pong(sun::context& p_context)
    :   sun::application(p_context),
        left_pad_(p_context),
        right_pad_(p_context),
        ball_(p_context)
    {
        window_.set_title("Pong!");

        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));

        left_pad_.shape.set_color(sun::color::sun);
        left_pad_.shape.set_shape(sun::shapes::rectangle(32.f, 128.f));
        left_pad_.transform.set_translation(0.f, 360.f - 64.f);

        right_pad_.shape.set_color(sun::color::sun);
        right_pad_.shape.set_shape(sun::shapes::rectangle(32.f, 128.f));
        right_pad_.transform.set_translation(1280.f - 32.f, 360.f - 64.f);

        ball_.shape.set_color(sun::color::sun);
        ball_.shape.set_shape(sun::shapes::circle(20.f, 120));
        ball_.transform.set_translation(640.f - 20.f, 360.f - 20.f);

        ball_.speed.x = 10.f;
        ball_.speed.y = 2.f;
    }

    void on_update()
    {
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::UP)) {
            right_pad_.transform.translate(0.f, -8.f);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::DOWN)) {
            right_pad_.transform.translate(0.f, 8.f);
        }

        if (sun::keyboard::is_key_pressed(sun::keyboard::key::A)) {
            left_pad_.transform.translate(0.f, -8.f);
        }
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::Z)) {
            left_pad_.transform.translate(0.f, 8.f);
        }

        /*renderer_->clear();

        renderer_->set_model_transform(left_pad_.transform);
        renderer_->draw(left_pad_.shape);

        renderer_->set_model_transform(right_pad_.transform);
        renderer_->draw(right_pad_.shape);

        ball_.transform.translate(ball_.speed);
        renderer_->set_model_transform(ball_.transform);
        renderer_->draw(ball_.shape);*/
    }

private:

    struct pad {
        sun::shape2D    shape;
        sun::matrix4    transform;

        pad(sun::context& p_context) : shape(p_context) {}
    };

    struct ball {
        sun::shape2D    shape;
        sun::matrix4    transform;
        sun::vector2f   speed;

        ball(sun::context& p_context) : shape(p_context) {}
    };

    pad     left_pad_;
    pad     right_pad_;
    ball    ball_;
};

SUN_DEFINE_MAIN_APP(pong);
