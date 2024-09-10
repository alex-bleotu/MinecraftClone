#ifndef MINECRAFTCLONE_USERINTERFACE_H
#define MINECRAFTCLONE_USERINTERFACE_H

#include <SFML/Graphics.hpp>

namespace UI {
    class Widget {
    protected:
        sf::Vector2f position;
        sf::Vector2f size;

    public:
        virtual void update(float& deltaTime) = 0;
        virtual void render(sf::RenderWindow& window) const = 0;

        [[nodiscard]] sf::Vector2f getPosition() const;
        [[nodiscard]] sf::Vector2f getSize() const;
        virtual void setPosition(sf::Vector2f position);
        virtual void setSize(sf::Vector2f size);
        virtual bool isClicked(sf::Vector2f mousePosition);
    };

    class Button : public Widget {
    private:
        sf::Color backgroundColor = sf::Color::Black;
        sf::Color textColor = sf::Color::White;
        sf::Color borderColor = sf::Color::White;

        float borderSize = 2;
        unsigned int fontSize = 24;

        std::string text;

        sf::RectangleShape border;
        sf::RectangleShape background;
        sf::Text buttonText;
    public:
        explicit Button(const std::string& text);

        void update(float& deltaTime) override;
        void render(sf::RenderWindow& window) const override;
        bool isClicked(sf::Vector2f mousePosition) override;

        void setPosition(sf::Vector2f position) override;
        void setSize(sf::Vector2f size) override;

        void setBackgroundColor(sf::Color color);
        void setTextColor(sf::Color color);
        void setBorderColor(sf::Color color);
        void setBorderSize(float size);
        void setFontSize(unsigned int size);
        void setText(std::string text);
    };

    class Label : public Widget {
    private:
        sf::Color textColor = sf::Color::White;
        unsigned int fontSize = 24;

        std::string text;

        sf::Text labelText;
    public:
        explicit Label(std::string text);

        void update(float& deltaTime) override;
        void render(sf::RenderWindow& window) const override;

        void setPosition(sf::Vector2f position) override;
        void setSize(sf::Vector2f size) override;

        void setTextColor(sf::Color color);
        void setFontSize(unsigned int size);
        void setText(std::string text);
    };

    class Layout : public Widget {
    private:
        std::vector<Widget*> widgets;
    public:
        explicit Layout(
                const std::vector<Widget*>& widgets,
                sf::Vector2f position = { 0, 0 }
        );
        ~Layout();

        void update(float& deltaTime) override;
        void render(sf::RenderWindow& window) const override;

        void addWidget(Widget* widget);
        [[nodiscard]] std::vector<Widget*> getWidgets() const;
    };
}


#endif
