#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Student {
    char name[20];
    int roll;
    int marks;
    char grade;
};

int main(int argc, char *argv[]) {

    int rank, size;
    int total_students = 0;
    int chunk;

    struct Student *students = NULL;
    struct Student *local_students = NULL;
    struct Student *result = NULL;

    MPI_Datatype MPI_STUDENT;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int blocklen[4] = {20, 1, 1, 1};
    MPI_Datatype types[4] = {MPI_CHAR, MPI_INT, MPI_INT, MPI_CHAR};
    MPI_Aint disp[4];

    struct Student temp;
    MPI_Aint base, a1, a2, a3, a4;

    MPI_Get_address(&temp, &base);
    MPI_Get_address(&temp.name, &a1);
    MPI_Get_address(&temp.roll, &a2);
    MPI_Get_address(&temp.marks, &a3);
    MPI_Get_address(&temp.grade, &a4);

    disp[0] = a1 - base;
    disp[1] = a2 - base;
    disp[2] = a3 - base;
    disp[3] = a4 - base;

    MPI_Type_create_struct(4, blocklen, disp, types, &MPI_STUDENT);
    MPI_Type_commit(&MPI_STUDENT);

   if (rank == 0) {

        FILE *fp = fopen("students.txt", "r");
        char temp_name[20];
        int r, m;

        for (;;) {
            if (fscanf(fp, "%s %d %d", temp_name, &r, &m) == EOF)
                break;
            total_students++;
        }

        rewind(fp);

        students = (struct Student *)malloc(total_students * sizeof(struct Student));
        result   = (struct Student *)malloc(total_students * sizeof(struct Student));

        for (int i = 0; i < total_students; i++) {
            fscanf(fp, "%s %d %d",
                   students[i].name,
                   &students[i].roll,
                   &students[i].marks);
        }

        fclose(fp);

        printf("Rank 0: Read %d students from file\n", total_students);
    }

    MPI_Bcast(&total_students, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = total_students / size;

    local_students = (struct Student *)malloc(chunk * sizeof(struct Student));

    MPI_Scatter(students, chunk, MPI_STUDENT,
                local_students, chunk, MPI_STUDENT,
                0, MPI_COMM_WORLD);

    printf("Rank %d: Received %d students\n", rank, chunk);

    for (int i = 0; i < chunk; i++) {

        if (local_students[i].marks >= 90)
            local_students[i].grade = 'A';
        else if (local_students[i].marks >= 75)
            local_students[i].grade = 'B';
        else if (local_students[i].marks >= 60)
            local_students[i].grade = 'C';
        else
            local_students[i].grade = 'F';
    }

    printf("Rank %d: Assigned grades to %d students\n", rank, chunk);

    MPI_Gather(local_students, chunk, MPI_STUDENT,
               result, chunk, MPI_STUDENT,
               0, MPI_COMM_WORLD);


    if (rank == 0) {

        FILE *out = fopen("grades.txt", "w");
        fprintf(out, "Name\tRoll\tMarks\tGrade\n");

        for (int i = 0; i < total_students; i++) {
            fprintf(out, "%s\t%d\t%d\t%c\n",
                    result[i].name,
                    result[i].roll,
                    result[i].marks,
                    result[i].grade);
        }

        fclose(out);

        printf("Grades written to file successfully.\n");

        free(students);
        free(result);

    }

    MPI_Type_free(&MPI_STUDENT);
    MPI_Finalize();
    return 0;
}
