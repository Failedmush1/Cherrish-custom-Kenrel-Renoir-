import re

file_path = 'drivers/staging/qca-wifi-host-cmn/dp/wifi3.0/dp_ipa.c'

with open(file_path, 'r') as f:
    content = f.read()

def patch_dp_ipa_setup(match):
    func_body = match.group(0)
    
    # 1. Change declarations
    func_body = re.sub(
        r'qdf_ipa_wdi_conn_in_params_t pipe_in;\n\s+qdf_ipa_wdi_conn_out_params_t pipe_out;',
        r'qdf_ipa_wdi_conn_in_params_t *pipe_in = NULL;\n\tqdf_ipa_wdi_conn_out_params_t *pipe_out = NULL;\n\tQDF_STATUS status = QDF_STATUS_SUCCESS;',
        func_body
    )
    
    # 2. Replace mem_zero with allocation
    func_body = re.sub(
        r'qdf_mem_zero\(&pipe_in, sizeof\(pipe_in\)\);\n\s+qdf_mem_zero\(&pipe_out, sizeof\(pipe_out\)\);',
        r'pipe_in = qdf_mem_malloc(sizeof(*pipe_in));\n\tpipe_out = qdf_mem_malloc(sizeof(*pipe_out));\n\tif (!pipe_in || !pipe_out) {\n\t\tstatus = QDF_STATUS_E_NOMEM;\n\t\tgoto free_pipes;\n\t}',
        func_body
    )
    
    # 3. Replace references
    func_body = func_body.replace('&pipe_in', 'pipe_in')
    func_body = func_body.replace('&pipe_out', 'pipe_out')
    
    # 4. Handle returns
    # We want to replace `return QDF_STATUS_E_FAILURE;` with `status = QDF_STATUS_E_FAILURE; goto free_pipes;`
    # But only inside the function body.
    func_body = re.sub(
        r'return\s+QDF_STATUS_E_FAILURE;',
        r'status = QDF_STATUS_E_FAILURE;\n\t\tgoto free_pipes;',
        func_body
    )
    
    # 5. Handle the final return QDF_STATUS_SUCCESS;
    # It might just be the last one. Let's find it.
    func_body = re.sub(
        r'return\s+QDF_STATUS_SUCCESS;\n\}',
        r'status = QDF_STATUS_SUCCESS;\n\nfree_pipes:\n\tif (pipe_in)\n\t\tqdf_mem_free(pipe_in);\n\tif (pipe_out)\n\t\tqdf_mem_free(pipe_out);\n\n\treturn status;\n}',
        func_body
    )
    
    # 6. Some early returns of QDF_STATUS_SUCCESS might exist (e.g. if (!wlan_cfg_is_ipa_enabled))
    # Wait, the allocation happens AFTER wlan_cfg_is_ipa_enabled check, so returning directly is fine.
    # Let's fix the early return if any:
    # Actually, the first early return is `if (!wlan_cfg_is_ipa_enabled...) return QDF_STATUS_SUCCESS;`
    # Because pipe_in and pipe_out are NULL, goto free_pipes would also work but `return` is fine since they are NULL and we didn't alloc yet.
    
    return func_body

# Find both dp_ipa_setup functions and patch them
content = re.sub(r'QDF_STATUS dp_ipa_setup\(.*?\n\}', patch_dp_ipa_setup, content, flags=re.DOTALL)

with open(file_path, 'w') as f:
    f.write(content)
