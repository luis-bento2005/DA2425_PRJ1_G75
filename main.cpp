#include <iostream>
#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    string folder = "../../DA2425_PRJ1_G75/For_Students";
    Graph<int> g = createGraphs::graphFromFile(folder);

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.