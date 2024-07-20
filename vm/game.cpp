#include "game.h"

vector<Card> Game::get_cards_of_type(string type)
{
	vector<Card> matching_cards;

	for (auto card_entry : cards)
	{
		Card card = card_entry.second;
		if (card.parentName == type)
		{
			matching_cards.push_back(card);
		}
	}

	return matching_cards;
}

std::string  Attr::ToString() {
    std::stringstream ss;

    if (type == AttributeType::BOOL) {
        ss << "BOOL " << b;
    }
    else if (type == AttributeType::INT) {
        ss << "INT " << i;
    }
    else if (type == AttributeType::FLOAT) {
        ss << "FLOAT " << f;
    }
    else if (type == AttributeType::STRING) {
        ss << "STRING " << s;
    }
    else if (type == AttributeType::CARD_REF) {
        ss << "CARD_REF " << cardRef;
    }
    else if (type == AttributeType::PLAYER_REF) {
        ss << "PLAYER_REF " << playerRef;
    }
    else if (type == AttributeType::STACK_REF) {
        ss << "STACK_REF " << stackRef;
    }
    else if (type == AttributeType::STACK_POSITION_REF) {
        ss << "STACK_POSITION_REF" << std::get<0>(stackPositionRef) << " " << std::get<1>(stackPositionRef);
    }

    return ss.str();
}

bool AttrCont::Contains(std::string name) {
    if (attrs.find(name) == attrs.end()) {
        return false;
    }
    return true;
}

void AttrCont::Store(std::string name, Attr a) {
    attrs[name] = a;
}

Attr& AttrCont::Get(std::string name) {
    return attrs[name];
}

std::string AttrCont::ToString(std::string prefix /* = "" */) {
    std::stringstream ss;
    for (auto pair : attrs) {
        ss << endl << prefix << pair.first << ": " << pair.second.ToString();
    }

    return ss.str();
}

void Game::Print() {
    cout << "Cards:" << endl;

    for (auto pair : cards) {
        cout << pair.first << ":" << pair.second.attributes.ToString("    ") << endl;
    }

    cout << "Stacks:" << endl;

    for (auto pair : stacks) {
        cout << pair.first << ":" << pair.second.attributes.ToString("    ") << endl;
    }

    cout << "Players:" << endl;

    for (int i = 0; i < players.size(); i++) {
        cout << i << ":" << players[i].attributes.ToString("    ") << endl;
    }

    cout << "Game Attributes:" << endl;

    cout << attributeCont.ToString("    ") << endl;
}