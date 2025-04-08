/**
 * @file main.c
 * @brief Test runner for the cmdproc module using the Unity framework.
 *
 * This file contains the main function used to execute unit tests for the command processor module.
 * It uses the Unity test framework and tracks the results of each test in order to present a
 * summary table at the end of execution.
 *
 * The tests include:
 * - Basic initialization of the module;
 * - Handling of valid and invalid UART commands;
 * - Checksum verification;
 * - Sensor value processing and conversion;
 * - Buffer management (transmit and receive);
 * - UART emulation functions.
 *
 * The test result summary is printed in a formatted table after all tests are executed.
 *
 * @author Simão Ribeiro
 * @date 2025-04-08
 *
 * @bug No known bugs.
 *
 * @see cmdproc.h
 */



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
    printf("\n================ RELATÓRIO FINAL ===============\n");
    printf("| %-35s | %-8s |\n", "TESTE", "ESTADO");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < test_count; i++) {
        printf("| %-35s | %-8s |\n", test_names[i], test_results[i]);
    }
    printf("==================================================\n");
    printf("|Número de testes: %d                            |\n",test_count);
    printf("==================================================\n");

    return UNITY_END();
}
