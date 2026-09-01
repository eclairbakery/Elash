#include "../prof-internals.h"

#include <elash/util/json.h>
#include <stdio.h>

static void print_stat_fields(FILE* out, const ElProfStat* stat) {
    fprintf(out, "\"wall-sec\":%.9f,\"user-sec\":%.9f,\"sys-sec\":%.9f",
            stat->wall_sec, stat->user_sec, stat->sys_sec);
}

void el_prof_print_jsonl(ElProfState* prof, FILE* out) {
    for (ElProfStage* stage = prof->head; stage != NULL; stage = stage->next) {
        ElProfStat stat = el_prof_stage_stat(stage);

        fputs("{\"type\":\"stage\",\"name\":", out);
        el_json_write_string(out, stage->name);
        fputc(',', out);
        print_stat_fields(out, &stat);
        fputs("}\n", out);

        for (ElProfSubstage* sub = stage->sub.head; sub != NULL; sub = sub->next) {
            fputs("{\"type\":\"substage\",\"stage\":", out);
            el_json_write_string(out, stage->name);
            fputs(",\"name\":", out);
            el_json_write_string(out, sub->name);
            fputc(',', out);
            print_stat_fields(out, &sub->total);
            fprintf(out, ",\"entered-count\":%u}\n", sub->entered_count);
        }

        ElProfStat other = el_prof_substage_other(stage, &stat);
        if (other.wall_sec > 0) {
            fputs("{\"type\":\"substage\",\"stage\":", out);
            el_json_write_string(out, stage->name);
            fputc(',', out);
            fputs("\"name\":\"(other)\",\"entered-count\":0,", out);
            print_stat_fields(out, &other);
            fputs("}\n", out);
        }
    }
}
