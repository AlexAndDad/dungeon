//
// Created by Richard Hodges on 17/01/2018.
//

static constexpr bool testing = true;

std::istream &choose_input() {
    if (testing) {
        static std::istringstream iss("Richard Hodges\n49\nJelly\n");
        return iss;
    } else {
        return std::cin;
    }
}

std::vector<std::string>
extract_spellbooks(std::vector<std::string> const &inventory) {
    using std::begin;
    using std::end;

    auto not_spellbook = [](std::string const &candidate) {
        static const char spellbook_[] = "spellbook";
        return std::search(begin(candidate), end(candidate),
                           begin(spellbook_), end(spellbook_))
               == end(candidate);
    };

    std::vector<std::string> result;
    std::remove_copy_if(begin(inventory), end(inventory),
                        back_inserter(result),
                        not_spellbook);
    return result;
}

