#include <criterion/criterion.h>

#include <elash/pp/scope.h>
#include <elash/pp/value.h>
#include <elash/defs/sv.h>

// NOLINTBEGIN(readability-magic-numbers)

Test(pp_scope, basic_usage) {
    ElPpScope scope;
    el_pp_scope_init(&scope, NULL, 8);

    ElPpVar var1 = {
        .name = EL_SV("foo"),
        .value = {
            .type = EL_PP_VAR_INT,
            .as.int_ = 42,
        },
    };

    ElStringView key = EL_SV("foo");
    cr_assert(el_pp_scope_assign(&scope, key, &var1));

    ElPpVar* lookup_result = el_pp_scope_lookup(&scope, key);
    cr_assert_not_null(lookup_result);
    cr_assert_eq(lookup_result, &var1);
    cr_assert_eq(lookup_result->value.as.int_, 42);

    cr_assert(el_pp_scope_deassign(&scope, key));
    cr_assert_null(el_pp_scope_lookup(&scope, key));

    el_pp_scope_destroy(&scope);
}

Test(pp_scope, nested_lookup) {
    ElPpScope parent, child;
    el_pp_scope_init(&parent, NULL, 8);
    el_pp_scope_init(&child, &parent, 8);

    ElPpVar var_parent = {
        .name = EL_SV("foo"),
        .value ={
            .type = EL_PP_VAR_INT,
            .as.int_ = 100,
        },
    };

    ElStringView key = EL_SV("foo");
    el_pp_scope_assign(&parent, key, &var_parent);

    ElPpVar* lookup = el_pp_scope_lookup(&child, key);
    cr_assert_not_null(lookup);
    cr_assert_eq(lookup, &var_parent, "child should see parent variable");

    el_pp_scope_destroy(&child);
    el_pp_scope_destroy(&parent);
}

Test(pp_scope, shadowing) {
    ElPpScope parent, child;
    el_pp_scope_init(&parent, NULL, 8);
    el_pp_scope_init(&child, &parent, 8);

    ElPpVar var_parent = {
        .name = EL_SV("foo"),
        .value = {
            .type = EL_PP_VAR_FLOAT,
            .as.float_ = 3.14f,
        },
    };

    ElPpVar var_child = {
        .name = EL_SV("foo"),
        .value = {
            .type = EL_PP_VAR_FLOAT,
            .as.float_ = 2.19f,
        },
    };

    ElStringView key = EL_SV("foo");
    el_pp_scope_assign(&parent, key, &var_parent);
    el_pp_scope_assign(&child, key, &var_child);

    ElPpVar* lookup_child = el_pp_scope_lookup(&child, key);
    cr_assert_eq(lookup_child, &var_child, "child should shadow parent");

    ElPpVar* lookup_parent = el_pp_scope_lookup(&parent, key);
    cr_assert_eq(lookup_parent, &var_parent, "parent should remain unaffected by shadowing");

    el_pp_scope_destroy(&child);
    el_pp_scope_destroy(&parent);
}

// NOLINTEND(readability-magic-numbers)
