
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>
#include <stack>


ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    // =================== COMMAND HANDLERS ===================
    std::string tokens[10];
    int count = tokenize(line, tokens, 10);

    if (count == 0) {
        return;
    }

    const std::string &cmd = tokens[0];

    if (cmd == "ADD_ARTIFACT") {
        handleAddArtifact(tokens, count);
    }
    else if (cmd == "REMOVE_ARTIFACT") {
        handleRemoveArtifact(tokens, count);
    }
    else if (cmd == "HIRE_RESEARCHER") {
        handleHireResearcher(tokens, count);
    }
    else if (cmd == "FIRE_RESEARCHER") {
        handleFireResearcher(tokens, count);
    }
    else if (cmd == "REQUEST") {
        handleRequest(tokens, count);
    }
    else if (cmd == "RETURN") {
        handleReturn(tokens, count);
    }
    else if (cmd == "RETURN_ALL") {
        handleReturnAll(tokens, count);
    }
    else if (cmd == "RESEARCHER_LOAD") {
        handleResearcherLoad(tokens, count);
    }
    else if (cmd == "MATCH_RARITY") {
        handleMatchRarity(tokens, count);
    }
    else if (cmd == "PRINT_UNASSIGNED") {
        handlePrintUnassigned(tokens, count);
    }
    else if (cmd == "PRINT_STATS") {
        handlePrintStats(tokens, count);
    }
    else if (cmd == "CLEAR") {
        handleClear(tokens, count);
    }
    else {
        std::cout << "Error: Unknown command " << "'" << cmd << "'." << std::endl;
    }
}


void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    if (count != 5) {
        return;
    }

    //convert strings from tokens for data processing
    int id = std::stoi(tokens[1]);
    std::string name = tokens[2];
    int rarity = std::stoi(tokens[3]);
    int value = std::stoi(tokens[4]);

    Artifact artifact(id, name, rarity, value);

    // Try to insert into the AVL Tree
    if (artifactTree.insertArtifact(artifact)) {
        std::cout << "Artifact " << id << " added." << std::endl;
    }
    else {
        std::cout << "Error: Artifact " << id << " already exists." << std::endl;
    }
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.
    if (count != 2) {
        return;
    }

    //convert strings from tokens for data processing
    int id = std::stoi(tokens[1]);
    ArtifactNode* artifact = artifactTree.findArtifact(id);

    if (!artifact) {
        std::cout << "Error: Artifact " << id << " not found." << std::endl;
        return;
    }

    const std::string& assignedName = artifact->data.assignedToName;

    if (!assignedName.empty()) {
        ResearcherNode* researcher =  researcherTree.findResearcher(assignedName);
        if (researcher) {
            researcher->data.removeArtifact(id);
        }
    }

    // Now safely remove the artifact from the AVL Tree
    artifactTree.removeArtifact(id);
    std::cout << "Artifact " << id << " removed." << std::endl;
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.
    if (count != 3) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    int capacity = std::stoi(tokens[2]);

    Researcher researcher(name, capacity);

    //try to insert the new researcher into the Red-Black Tree
    if (researcherTree.insertResearcher(researcher)) {
        std::cout << "Researcher " << name << " hired." << std::endl;
    }
    else {
        std::cout << "Error: Researcher " << name << " already exists." << std::endl;
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
    if (count != 2) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    ResearcherNode *researcher = researcherTree.findResearcher(name);

    if (!researcher) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }

    //before firing find all artifacts they supervise and mark them as UNASSIGNED in the AVL
    for (int i = 0; i < researcher->data.numAssigned; i++) {
        artifactTree.clearAssignedTo(researcher->data.assignedArtifacts[i]);
    }

    //remove the researcher from the Red-Black Tree
    researcherTree.removeResearcher(name);
    std::cout << "Researcher " << name << " fired." << std::endl;
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."
    if (count != 3) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    int id = std::stoi(tokens[2]);

    ResearcherNode *researcher = researcherTree.findResearcher(name);

    //check if researcher and artifact exist
    if (!researcher) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }

    ArtifactNode *artifact = artifactTree.findArtifact(id);

    if (!artifact) {
        std::cout << "Error: Artifact " << id << " not found." << std::endl;
        return;
    }

    // Check if the artifact is free
    if (!artifact->data.assignedToName.empty()) {
        std::cout << "Error: Artifact " << id << " already assigned." << std::endl;
        return;
    }

    if (!researcher->data.addArtifact(id)) {
        std::cout << "Error: Researcher " << name << " is at full capacity." << std::endl;
        return;
    }

    //link the name in AVL and boost the artifact's value
    artifactTree.setAssignedTo(id, name);
    artifact->data.updateValueBasedOnUsage();

    std::cout << "Artifact " << id << " assigned to " << name << "." << std::endl;

}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."
    if (count != 3) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    int id = std::stoi(tokens[2]);

    ResearcherNode *researcher = researcherTree.findResearcher(name);
    ArtifactNode *artifact = artifactTree.findArtifact(id);

    //ensure the return is valid
    if (!researcher || !artifact || artifact->data.assignedToName != name) {
        std::cout << "Error: Invalid return operation." << std::endl;
        return;
    }

    //unlink the artifact from both the researcher's list and the AVL node
    researcher->data.removeArtifact(id);
    artifactTree.clearAssignedTo(id);

    std::cout << "Artifact " << id << " returned by " << name << "." << std::endl;
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.
    if (count != 2) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    ResearcherNode *researcher = researcherTree.findResearcher(name);

    if (!researcher) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }

    for(int i = 0; i < researcher->data.numAssigned; i++) {
        artifactTree.clearAssignedTo(researcher->data.assignedArtifacts[i]);
    }

    //reset the researcher's internal count to 0
    researcher->data.removeAllArtifacts();
    std::cout << "All artifacts returned by " << name << "." << std::endl;
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
    if (count != 2) {
        return;
    }

    //convert strings from tokens for data processing
    std::string name = tokens[1];
    ResearcherNode *researcher = researcherTree.findResearcher(name);

    if (!researcher) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
    }
    else {
        std::cout << "Researcher " << name << " supervises " << researcher->data.numAssigned << " artifacts." << std::endl;
    }
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.
    if (count != 2) {
        return;
    }

    //convert strings from tokens for data processing
    int minimumRarity = std::stoi(tokens[1]);

    std::cout << "=== MATCH_RARITY " << minimumRarity << " ===" << std::endl;

    //manual In-order traversal using a Stack to find matching rarity levels
    std::stack<ArtifactNode*> st;
    ArtifactNode* current = artifactTree.getRoot();

    while (current != nullptr || !st.empty()) {

        while (current != nullptr) {
            st.push(current);
            current = current->left;
        }

        current = st.top();
        st.pop();

        //print only if the artifact meets the minimum rarity criteria
        if (current->data.rarityLevel >= minimumRarity) {
            if (current->data.assignedToName.empty()) {
                std::cout << current->data.artifactID << " " << current->data.name << " " << current->data.rarityLevel << " " << current->data.researchValue << " UNASSIGNED" << std::endl;
            }
            else {
                std::cout << current->data.artifactID << " " << current->data.name << " " << current->data.rarityLevel << " " << current->data.researchValue << " ASSIGNED:" << current->data.assignedToName << std::endl;
            }
        }

        current = current->right;
    }
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    std::cout << "Unassigned artifacts:" << std::endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)

    int a = artifactTree.getArtifactCount();
    int r = researcherTree.getResearcherCount();

    //calculate averages
    int averageRarity;
    if (a == 0) {
        averageRarity = 0;
    }
    else {
        averageRarity = artifactTree.getTotalRarity() / a;
    }

    int averageLoad;
    if (r == 0) {
        averageLoad = 0;
    }
    else {
        averageLoad = researcherTree.getTotalLoad() / r;
    }

    std::cout << "=== SYSTEM STATISTICS ===" << std::endl;
    std::cout << "Artifacts: " << a << std::endl;
    std::cout << "Researchers: " << r << std::endl;
    std::cout << "Average rarity: " << averageRarity << std::endl;
    std::cout << "Average load: " << averageLoad << std::endl;

    std::cout << "Researchers:" << std::endl;
    researcherTree.traversePreOrderForStats();
    std::cout << "Artifacts:" << std::endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    artifactTree.clear();
    researcherTree.clear();
    // e.g. "All data cleared."
    std::cout << "All data cleared." << std::endl;
}
