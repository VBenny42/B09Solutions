Student *insert_new_student(Student *list, int newStudentNo, char *newName) {
    if (list == NULL) {
        Student *node = (Student *) calloc(1, sizeof(Student));
        node->last_name = (char *) calloc(strlen(newName), sizeof(char));
        strcpy(node->last_name, newName);
        node->student_number = newStudentNo;
        node->next = NULL;
        return node;
    }
    for (Student *temp = list; temp != NULL; temp = temp->next) {
        if ((strcmp(temp->last_name, newName) < 0)) {
            Student *node = (Student *) calloc(1, sizeof(Student));
            node->last_name = (char *) calloc(strlen(newName), sizeof(char));
            strcpy(node->last_name, newName);
            node->student_number = newStudentNo;
            node->next = temp->next;
            temp->next = node;
            return list;
        }
    }
}