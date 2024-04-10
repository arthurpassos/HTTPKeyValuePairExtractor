#include <iostream>
#include <KeyValuePairExtractorBuilder.h>

int main() {
    auto kvp_extractor = KeyValuePairExtractorBuilder().build();

    auto result = kvp_extractor->extract("name:arthur");

    for (const auto & [key, value] : result) {
        std::cout << key << " -> " << value << std::endl;
    }
    return 0;
}
