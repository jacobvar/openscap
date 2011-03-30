/*
 *
 * Example program that read OVAL content (argv[1]) and
 * evaluate all objects in it. The output oval results xml file.
 *
 * author: Peter Vrabec <pvrabec@redhat.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oval_agent_api.h>
#include "../../assume.h"
#include <error.h>

int main(int argc, char **argv) {
  struct oval_sysinfo *sysinfo = NULL;
  int ret;

  /* definition model populate */
  struct oval_definition_model *def_model=NULL;
  def_model = oval_definition_model_import(argv[1]);
  assume(def_model != NULL);

  assume(oval_definition_model_is_valid(def_model));

  /* create syschar model */
  struct oval_syschar_model *sys_model = oval_syschar_model_new(def_model);
  assume(sys_model != NULL);

  /* create probe session */
  oval_probe_session_t *sess = oval_probe_session_new(sys_model);

  /* probe sysinfo */
  ret = oval_probe_query_sysinfo(sess, &sysinfo);
  assume(ret == 0 && sysinfo != NULL);
  oval_syschar_model_set_sysinfo(sys_model, sysinfo);
  oval_sysinfo_free(sysinfo);

  /* call probes */
  assume(oval_probe_query_objects(sess) == 0);

  /* create result model */
  struct oval_syschar_model *sys_models[] = {sys_model, NULL};
  struct oval_results_model* res_model = oval_results_model_new( def_model, sys_models );
  assume(res_model != NULL);

  /* evaluate gathered system characteristics */
  oval_results_model_eval(res_model);

  /* set up directives */
  struct oval_result_directives * res_direct = oval_result_directives_new(res_model);
  assume(res_direct != NULL);

  oval_result_directives_set_reported(res_direct, OVAL_RESULT_TRUE, true);
  oval_result_directives_set_reported(res_direct, OVAL_RESULT_FALSE, true);
  oval_result_directives_set_reported(res_direct, OVAL_RESULT_UNKNOWN, true);
  oval_result_directives_set_reported(res_direct, OVAL_RESULT_ERROR, true);
  oval_result_directives_set_reported(res_direct, OVAL_RESULT_NOT_EVALUATED, true);
  oval_result_directives_set_reported(res_direct, OVAL_RESULT_NOT_APPLICABLE , true);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_FALSE, OVAL_DIRECTIVE_CONTENT_FULL);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_TRUE, OVAL_DIRECTIVE_CONTENT_FULL);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_UNKNOWN, OVAL_DIRECTIVE_CONTENT_FULL);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_ERROR, OVAL_DIRECTIVE_CONTENT_FULL);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_NOT_EVALUATED, OVAL_DIRECTIVE_CONTENT_FULL);
  oval_result_directives_set_content(res_direct,OVAL_RESULT_NOT_APPLICABLE, OVAL_DIRECTIVE_CONTENT_FULL);

  /* report results */
  oval_results_model_export(res_model, res_direct, argv[2]);

  /* free */
  oval_probe_session_destroy(sess);
  oval_definition_model_free(def_model);
  oval_syschar_model_free(sys_model);
  oval_results_model_free(res_model);
  oval_result_directives_free(res_direct);
  oscap_cleanup();
  oscap_clearerr();

  return 0;
}

/* /\* */
/*  * test_probes.c */
/*  * */
/*  *  Created on: Aug 4, 2009 */
/*  *      Author: Peter Vrabec */
/*  *\/ */

/* #include <stdio.h> */
/* #include <string.h> */
/* #include <stdlib.h> */
/* #include "oval_agent_api.h" */
/* #include "oval_definitions_impl.h" */
/* #include "oval_system_characteristics_impl.h" */
/* #include "error.h" */

/* int _test_error() */
/* { */
/*         if (oscap_err ()) { */
/*                 oscap_errfamily_t f; */
/*                 oscap_errcode_t   c; */
/*                 const char       *d; */

/*                 fprintf (stderr, "GOT error: %d, %d, %s.\n", */
/*                         f = oscap_err_family (), */
/*                         c = oscap_err_code (), */
/*                         d = oscap_err_desc ()); */
/*         } */

/* 	return 0; */
/* } */

/* int _test_error_handler(struct oval_xml_error *error, void *null) /\* <-- deprecated *\/ */
/* { */
/* 	return 1; */
/* } */

/* void oval_syschar_to_print(struct oval_syschar*, const char*, int); */

/* int main(int argc, char **argv) */
/* { */
/* 	int idx, ret; */
/*         oval_pctx_t *pctx; */
/* 	char *id, *probe_dir; */
/* 	struct oval_definition_model *definition_model; */
/* 	struct oval_syschar_model *syschar_model; */
/* 	struct oscap_import_source *source; */
/* 	struct oval_object_iterator *obj_it; */
/* 	struct oval_object *object; */
/* 	struct oval_syschar *syschar; */

/* 	if (argc != 2) { */
/* 		printf("usage: %s <oval_definitions.xml>\n", argv[0]); */
/* 		return 1; */
/* 	} */

/* 	source = oscap_import_source_new_file(argv[1], NULL); */
/*         _test_error(); */
/* 	definition_model = oval_definition_model_new(); */
/* 	ret = oval_definition_model_import(definition_model, source, NULL); */
/* 	if(ret != 1) { */
/* 		printf("oval_definition_model_import() failed.\n"); */
/*                 _test_error(); */
/* 		return 2; */
/* 	} */
/* 	oscap_import_source_free(source); */
/* 	syschar_model = oval_syschar_model_new(definition_model); */

/* 	obj_it = oval_definition_model_get_objects(definition_model); */
/* 	if (!oval_object_iterator_has_more(obj_it)) { */
/* 		printf("No definitions found.\n"); */
/* 		return 3; */
/* 	} */

/* 	probe_dir = getenv ("OVAL_PROBE_DIR"); */
/* 	if (probe_dir == NULL) { */
/* 		printf("'OVAL_PROBE_DIR' not found in environment.\n"); */
/* 		return 4; */
/* 	} */
/* 	printf("probe_dir: %s.\n", probe_dir); */
/*         pctx = oval_pctx_new (syschar_model); */
/* 	if (pctx == NULL) { */
/* 		printf("oval_pctx_new() failed.\n"); */
/* 		return 5; */
/* 	} */
/*         oval_pctx_setattr (pctx, OVAL_PCTX_ATTR_DIR, probe_dir); */

/* 	for (idx = 1; oval_object_iterator_has_more(obj_it); idx++) { */
/* 		object = oval_object_iterator_next(obj_it); */
/* 		id = oval_object_get_id(object); */

/* 		printf("=== Object definition ===\n"); */
/* 		oval_object_to_print(object, "    ", idx); */

/* 		syschar = oval_syschar_model_get_syschar(syschar_model, id); */

/* 		if (syschar == NULL) { */
/* 			printf("=== Calling probe on object ===\n"); */
/* 			syschar = oval_probe_object_eval(pctx, object); */

/* 			if (syschar == NULL) { */
/* 				syschar = oval_syschar_new(syschar_model, object); */
/* 				oval_syschar_set_flag(syschar, SYSCHAR_FLAG_NOT_COLLECTED); */
/* 			} */

/* 			oval_syschar_model_add_syschar(syschar_model, syschar); */
/* 		} */

/* 		printf("=== System characteristics ===\n"); */
/* 		oval_syschar_to_print(syschar, "    ", idx); */
/* 	} */

/*         oval_pctx_free (pctx); */
/* 	oval_object_iterator_free(obj_it); */
/*         oval_definition_model_free(definition_model); */
/*         oval_syschar_model_free(syschar_model); */


/* 	return 0; */
/* } */

/* void oval_syschar_to_print(struct oval_syschar *syschar, const char *indent, int idx) */
/* { */
/*        char nxtindent[100]; */

/*        if (strlen(indent) > 80) */
/*                indent = "...."; */
/*        if (idx == 0) */
/*                snprintf(nxtindent, sizeof(nxtindent), "%sSYSCHAR.", indent); */
/*        else */
/*                snprintf(nxtindent, sizeof(nxtindent), "%sSYSCHAR[%d].", indent, idx); */

/*        /\* */
/*        oval_syschar_collection_flag_enum flag; */
/*        struct oval_collection *messages; */
/*        struct oval_sysinfo *sysinfo; */
/*        struct oval_object *object; */
/*        struct oval_collection *sysdata; */
/*         *\/ */
/*        printf("%sFLAG    = %d\n", nxtindent, oval_syschar_get_flag(syschar)); */
/*       {//messages */
/*                struct oval_message_iterator *messages = oval_syschar_get_messages(syschar); */
/*                int i;for(i=1;oval_message_iterator_has_more(messages);i++){ */
/*                        struct oval_message *message = oval_message_iterator_next(messages); */
/*                        oval_message_to_print(message, nxtindent, i); */
/*                } */
/*                oval_message_iterator_free(messages); */
/*        } */
/*        {//object */
/*                struct oval_object *object = oval_syschar_get_object(syschar); */
/*                if (object) oval_object_to_print(object, nxtindent, 0); */
/*        } */
/*        {//sysdata */
/*                struct oval_sysitem_iterator *sysdatas = oval_syschar_get_sysdata(syschar); */
/*                int hasMore = oval_sysitem_iterator_has_more(sysdatas); */
/*                if(hasMore){ */
/*                        int i;for(i=1;oval_sysitem_iterator_has_more(sysdatas);i++){ */
/*                                struct oval_sysitem *sysdata = oval_sysitem_iterator_next(sysdatas); */
/*                                oval_sysitem_to_print(sysdata, nxtindent, i); */
/*                       } */
/*                } */
/*                oval_sysitem_iterator_free(sysdatas); */
/*        } */
/* } */
