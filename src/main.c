#include <stdio.h>
#include "cmdproc.h"
#include "../unity/unity.h"
#include "../tests/test.h"

#define MAX_TESTS 50
const char* test_names[MAX_TESTS];
const char* test_results[MAX_TESTS];
int test_count = 0;

#define RUN_TEST_TRACKED(func)  \
    test_names[test_count] = #func; \
    RUN_TEST(func); \
    test_results[test_count] = Unity.CurrentTestFailed ? "FAIL" : "PASS"; \
    test_count++;

int main(void) 
{
    setUp();
    UNITY_BEGIN();

    RUN_TEST_TRACKED(test_cmdproc_init);
    RUN_TEST_TRACKED(test_command_A);
    RUN_TEST_TRACKED(test_command_P);
    RUN_TEST_TRACKED(test_command_L);
    RUN_TEST_TRACKED(test_command_R);
    RUN_TEST_TRACKED(test_user_cmd);
    RUN_TEST_TRACKED(test_nonexistent_cmd);
    RUN_TEST_TRACKED(test_wrong_values);
    RUN_TEST_TRACKED(test_wrong_checksum);
    RUN_TEST_TRACKED(test_num2char);
    RUN_TEST_TRACKED(test_char2num);
    RUN_TEST_TRACKED(test_rbuff);
    RUN_TEST_TRACKED(test_tbuff);
    RUN_TEST_TRACKED(teste_txchar);
    RUN_TEST_TRACKED(teste_rxchar);
    RUN_TEST_TRACKED(test_addValue);
    RUN_TEST_TRACKED(test_getsensor);

    tearDown();

    // Impressão em formato de tabela
    printf("\n======================= RELATÓRIO FINAL ==========\n");
    printf("| %-35s | %-8s |\n", "TESTE", "ESTADO");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < test_count; i++) {
        printf("| %-35s | %-8s |\n", test_names[i], test_results[i]);
    }
    printf("==================================================\n");
    
    return UNITY_END();
}
