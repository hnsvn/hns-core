# Copyright (c) 2021 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

import codecs
import os.path

# pylint: disable=import-error
from mojom.parse import ast
from mojom.parse import conditional_features
from mojom.parse import parser

# Attribute constant to mark a definition as a new one. If a definition with
# same name already exists, it's an error to add a new one.
_DEFINITION_ADD = 1
# Attribute constant to mark a definition as the one to extend. If a definition
# with same name and type doesn't exist, it's an error to extend one.
_DEFINITION_EXTEND = 2


# Predicate for an import filename.
def _AstImportPred(hns_import, ast_import):
    return hns_import.import_filename == ast_import.import_filename


# Predicate for a definition name (doesn't check type).
def _AstDefinitionNamePred(hns_definition, ast_definition):
    return hns_definition.mojom_name == ast_definition.mojom_name


# Predicate for definition name and type.
def _AstDefinitionPred(hns_definition, ast_definition):
    return _AstDefinitionNamePred(hns_definition, ast_definition) and \
              isinstance(hns_definition, type(ast_definition))


# Returns required action from a definition attributes list.
def _GetHnsDefinitionAction(hns_definition):
    if isinstance(hns_definition, ast.Const):
        # Const can only be added.
        return _DEFINITION_ADD

    if hns_definition.attribute_list:
        for attribute in hns_definition.attribute_list:
            if attribute.key == 'HnsAdd':
                return _DEFINITION_ADD
            if attribute.key == 'HnsExtend':
                return _DEFINITION_EXTEND

    raise ValueError(
        "Definition should have [HnsAdd] or [HnsExtend] attribute: %s" %
        hns_definition.mojom_name)


# Validates that a definition with same name doesn't exist in the list.
def _CheckDefinitionDoesntExist(hns_definition, ast_items):
    if any(
            _AstDefinitionNamePred(hns_definition, item)
            for item in ast_items):
        raise ValueError("Definition already exists: %s" %
                         hns_definition.mojom_name)


# Finds a definition by name and type.
def _FindMatchingDefinition(hns_definition, ast_definitions):
    for ast_definition in ast_definitions:
        if _AstDefinitionPred(hns_definition, ast_definition):
            return ast_definition

    return None


# Adds new items to an existing mojom definition.
def _ExtendAstDefinition(hns_definition, ast_definition):
    if isinstance(hns_definition, ast.Enum):
        for item in hns_definition.enum_value_list:
            _CheckDefinitionDoesntExist(item, ast_definition.enum_value_list)
            ast_definition.enum_value_list.Append(item)
    elif isinstance(hns_definition, (ast.Interface, ast.Struct, ast.Union)):
        items_to_append = []
        for item in reversed(hns_definition.body.items):
            if isinstance(item, (ast.Const, ast.Enum)):
                # Handle nested types.
                _ApplyHnsDefinition(item, ast_definition.body)
            else:
                _CheckDefinitionDoesntExist(item, ast_definition.body)
                items_to_append.append(item)
        # Restore members order and append them as declared.
        for item in reversed(items_to_append):
            ast_definition.body.Append(item)
    else:
        raise ValueError("Unhandled definition: %s" %
                         hns_definition.mojom_name)


# Adds or extends mojom ast definition.
def _ApplyHnsDefinition(hns_definition, ast_definitions):
    definition_action = _GetHnsDefinitionAction(hns_definition)
    if definition_action == _DEFINITION_ADD:
        _CheckDefinitionDoesntExist(hns_definition, ast_definitions)
        if isinstance(ast_definitions, ast.NodeListBase):
            ast_definitions.Insert(hns_definition)
        else:
            ast_definitions.insert(0, hns_definition)
    elif definition_action == _DEFINITION_EXTEND:
        ast_definition_to_extend = _FindMatchingDefinition(
            hns_definition, ast_definitions)
        if not ast_definition_to_extend:
            raise ValueError("Trying to extend non-existent definition: %s" %
                             hns_definition.mojom_name)
        _ExtendAstDefinition(hns_definition, ast_definition_to_extend)
    else:
        raise ValueError("Unknown definition action requested: %s" %
                         hns_definition.mojom_name)


# Applies changes to original mojom ast using hns ast.
def _ApplyHnsAstChanges(hns_ast, parsed_ast):
    # Make sure the module name is correct.
    if hns_ast.module != parsed_ast.module:
        raise ValueError(
            f"Mojo module ids are not equal while trying to patch: "
            f"{hns_ast.module.mojom_namespace} vs "
            f"{ast.module.mojom_namespace}")

    # Add new imports.
    for hns_import in hns_ast.import_list:
        if not any(
                _AstImportPred(hns_import, imp)
                for imp in parsed_ast.import_list):
            parsed_ast.import_list.Append(hns_import)

    # Add/extend mojo definitions and keep the type dependency order valid.
    #
    # At a later stage a mojo generator expects all types to be in order of use.
    # To acknowledge this we insert all new definitions in reversed order at
    # 0-position which will effectively place them in the right order as it was
    # declared in chromium_src/**/*.mojom *before* all existing definitions in
    # the original mojom.
    #
    # Enum values, struct/union members, interface methods are always appended.
    for hns_definition in reversed(hns_ast.definition_list):
        _ApplyHnsDefinition(hns_definition, parsed_ast.definition_list)


# Search for mojom file in 'chromium_src' and apply AST changes.
#
# Can add imports, enums, interfaces, structs, unions. Can extend enum values,
# interface members, struct fields, union members.
#
# To be more strict with patching, attributes [HnsAdd] or [HnsExtend] should
# be used.
def _PatchInHnsMojomAst(mojom_abspath, parsed_ast, enabled_features):
    # Get this script absolute location.
    this_py_path = os.path.realpath(__file__)

    # Get the original chromium dir location.
    chromium_original_dir = os.path.abspath(
        os.path.join(this_py_path, *[os.pardir] * 5))

    if len(chromium_original_dir) >= len(mojom_abspath) + 1:
        raise RuntimeError("Could not get original chromium src dir")

    # Build hns/chromium_src path.
    chromium_src_abspath = os.path.join(chromium_original_dir, 'hns',
                                        'chromium_src')
    if not os.path.isdir(chromium_src_abspath):
        raise RuntimeError(
            "Could not find hns/chromium_src. %s is not a dir" %
            chromium_src_abspath)

    # Relative path.
    mojom_relpath = mojom_abspath[len(chromium_original_dir) + 1:]

    # Build possible hns/chromium_src/**/*.mojom path.
    hns_mojom_abspath = os.path.join(chromium_src_abspath, mojom_relpath)
    if not os.path.isfile(hns_mojom_abspath):
        # Nothing to patch.
        return

    # Open and parse hns/chromium_src/**/*.mojom file.
    with codecs.open(hns_mojom_abspath, encoding='utf-8') as f:
        hns_ast = parser.Parse(f.read(), hns_mojom_abspath)
        conditional_features.RemoveDisabledDefinitions(hns_ast,
                                                       enabled_features)

        _ApplyHnsAstChanges(hns_ast, parsed_ast)


# Can't use @override_utils, because mojom_parser is running via multiprocessing
# which can't restore local variables required for @override_utils to work.
assert '_ParseAstHelper' in globals()
orig_parse_ast_helper = _ParseAstHelper


def _ParseAstHelper(mojom_abspath, enabled_features):
    mojom_abspath, parsed_ast = orig_parse_ast_helper(mojom_abspath,
                                                      enabled_features)
    _PatchInHnsMojomAst(mojom_abspath, parsed_ast, enabled_features)
    return mojom_abspath, parsed_ast
