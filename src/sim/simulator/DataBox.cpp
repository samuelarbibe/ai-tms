//
// Created by Samuel Arbibe on 07/02/2020.
//

#include "DataBox.hpp"

Font DataBox::font_{};
bool DataBox::font_loaded_ = false;

DataBox::DataBox(Vector2f position) : RectangleShape() {
	// will be displayed [offset] pixels above target
	max_data_items_ = 3;
	data_count_ = 0;
	offset_ = Vector2f(-0.f, -0.f);
	this->setPosition(Vector2f(position.x, position.y) + offset_);
	this->setSize(Vector2f(150.f, 50.f));
	this->setFillColor(Color::White);
	this->setOutlineColor(Color::Blue);
	this->setOutlineThickness(4.f);

	// load font if needed
	if (!font_loaded_)
	{
		if (!font_.loadFromFile("../../resources/fonts/Roboto/Roboto-Bold.ttf"))
		{
			cout << "ERROR: Could not load fond from the given file." << endl;
		}
	}
}

void DataBox::Update(Vector2f position) {
	this->setPosition(Vector2f(position.x, position.y) + offset_);
}

void DataBox::Draw(RenderWindow *window) {
	window->draw(*this);
	int count = 0;
	float space = 35;
	this->setSize(Vector2f(this->getSize().x, space * data_count_ + 5.f));

	Text temp;
	string s;
	temp.setCharacterSize(space - 5);
	temp.setFillColor(Color::Red);
	temp.setFont(font_);

	for (auto &data_item : data_.items())
	{
		s = data_item.key() + ": " + to_string(int(data_item.value()));
		temp.setString(s);
		temp.setPosition(Vector2f(this->getPosition().x + 10.f, this->getPosition().y + space * count));
		//temp.setPosition(Vector2f(this->getPosition().x + 10.f, this->getPosition().y - 30.f));

		window->draw(temp);
		count++;
	}
}

bool DataBox::AddData(string valueName, float value) {
	if (data_.size() < max_data_items_)
	{
		data_[valueName] = value;
		data_count_++;
		return true;
	}
	cout << "Could not add another data item into data box, max items reached." << endl;
	return false;
}

bool DataBox::SetData(string valueName, float value) {
	if (data_[valueName] != nullptr)
	{
		data_[valueName] = value;
		return true;
	}

	cout << "Could not set databox data item, as data name was not found." << endl;
	return false;
}

bool DataBox::RemoveData(string valueName) {
	if (data_[valueName] != nullptr)
	{
		data_.erase("valueName");
		return true;
	}

	cout << "Could not remove databox data item, as data name was not found." << endl;
	return false;
}
