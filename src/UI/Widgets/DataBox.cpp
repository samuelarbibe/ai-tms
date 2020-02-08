//
// Created by Samuel Arbibe on 07/02/2020.
//

#include "DataBox.hpp"

DataBox::DataBox(Vector2f position) : RectangleShape()
{
    // will be displayed [offset] pixels above target
    m_maxDataItems = 3;
    m_itemCount = 0;
    m_offset = Vector2f(-0.f, -0.f);
    this->setPosition(Vector2f(position.x , position.y) + m_offset);
    this->setSize(Vector2f(150.f, 50.f));
    this->setFillColor(Color::White);
    this->setOutlineColor(Color::Blue);
    this->setOutlineThickness(4.f);

    if (!m_font.loadFromFile("../../resources/Fonts/Roboto/Roboto-Bold.ttf"))
    {
        cout << "ERROR: Could not load fond from the given file." << endl;
    }
}

void DataBox::Update(Vector2f position)
{
    this->setPosition(Vector2f(position.x , position.y) + m_offset);
}

void DataBox::Draw(RenderWindow *window)
{
    window->draw(*this);
    int count = 0;
    float space = 35;
    this->setSize(Vector2f(this->getSize().x , space * m_itemCount + 5.f));

    Text temp;
    string s;
    temp.setCharacterSize(space-5);
    temp.setFillColor(Color::Red);
    temp.setFont(m_font);

    for(auto& data_item : m_data.items())
    {
        s = data_item.key() + ": " + to_string(int(data_item.value()));
        temp.setString(s);
        temp.setPosition(Vector2f(this->getPosition().x + 10.f, this->getPosition().y + space * count));
        //temp.setPosition(Vector2f(this->getPosition().x + 10.f, this->getPosition().y - 30.f));

        window->draw(temp);
        count ++;
    }
}

bool DataBox::AddData(string valueName, float value)
{
    if(m_data.size() < m_maxDataItems)
    {
        m_data[valueName] = value;
        m_itemCount++;
        return true;
    }
    cout << "Could not add another data item into data box, max items reached." << endl;
    return false;
}

bool DataBox::SetData(string valueName, float value)
{
   if(m_data[valueName] != nullptr)
   {
       m_data[valueName] = value;
       return true;
   }

   cout << "Could not set databox data item, as data name was not found." << endl;
   return false;
}

bool DataBox::RemoveData(string valueName)
{
    m_data.erase("valueName");
}
