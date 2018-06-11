PROJECT(gherkin C)
cmake_minimum_required(VERSION 3.0)
# Install library & headers in your directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
LIST(APPEND GHERKIN_SRS
        src/file_reader.c
        src/file_utf8_source.c
        src/print_utilities.c
        src/string_utilities.c
        src/unicode_utilities.c
        src/utf8_source.c
        src/parser.c
        src/token_scanner.c
        src/file_token_scanner.c
        src/string_token_scanner.c
        src/token_matcher.c
        src/token.c
        src/token_queue.c
        src/item_queue.c
        src/gherkin_line.c
        src/error.c
        src/error_list.c
        src/dialect.c
        src/ast_builder.c
        src/ast_node.c
        src/gherkin_document.c
        src/feature.c
        src/scenario.c
        src/scenario_outline.c
        src/background.c
        src/comment.c
        src/data_table.c
        src/doc_string.c
        src/example_table.c
        src/step.c
        src/table_cell.c
        src/table_row.c
        src/tag.c
        src/compiler.c
        src/pickle.c
        src/pickle_cell.c
        src/pickle_location.c
        src/pickle_row.c
        src/pickle_step.c
        src/pickle_string.c
        src/pickle_table.c
        src/pickle_tag.c
        src/event.c
        src/attachment_event.c
        src/gherkin_document_event.c
        src/pickle_event.c
        src/source_event.c
        src/ast_printer.c
        src/pickle_printer.c
        )

LIST(APPEND GENERATE_TOKEN
        src/token_formatter_builder.c
        src/gherkin_generate_tokens.c
        )
LIST(APPEND GHERKIN_CLI
        src/gherkin_cli.c
        )


add_library(gherkin SHARED ${GHERKIN_SRS})
target_link_libraries(gherkin PRIVATE m)
target_include_directories(gherkin PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/src>")

add_executable(gherkinexe ${GHERKIN_CLI})
target_link_libraries(gherkinexe PRIVATE m gherkin)
target_include_directories(gherkinexe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(gherkin_generate_tokens ${GENERATE_TOKEN})
target_link_libraries(gherkin_generate_tokens PRIVATE m gherkin)

file(GLOB GOOD_FEATURE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/testdata/good/*.feature
        )
file(GLOB BAD_FEATURE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/testdata/bad/*.feature
        )
add_custom_target(invo DEPENDS invo)
add_dependencies(invo gherkinexe)
FOREACH(ENTITY ${GOOD_FEATURE_FILES})
    add_custom_command(
            TARGET gherkinexe
            POST_BUILD
            COMMAND gherkinexe  ${ENTITY}
            DEPENDS invo
    )
ENDFOREACH()

############ Installation section ############
install(
  TARGETS gherkin
  EXPORT gherkin-config
  ARCHIVE DESTINATION lib/
  LIBRARY DESTINATION lib/
  RUNTIME DESTINATION lib/
  INCLUDES DESTINATION include)

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
  DESTINATION include
  FILES_MATCHING PATTERN "*.*h")

install(
    EXPORT gherkin-config
    FILE gherkin-config.cmake
    NAMESPACE gherkin::
    DESTINATION share/gherkin/cmake)