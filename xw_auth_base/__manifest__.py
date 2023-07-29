# -*- coding: utf-8 -*-
{
    'name':
        "xw_auth_base",
    'summary':
        """
        xw_auth_base
    """,
    'description':
        """
        xw auth base for odoo
    """,
    'author':
        "xw odoo",
    'website':
        "http://www.xwodoo.com",
    'category':
        'apps',
    'version':
        '14.0.0.1',
    'depends': ['base'],
    'external_dependencies': {
        'python': ['xw_cbase', 'pycryptodome']
    },
    'data': [
        'security/ir.model.access.csv',
        'views/reg_wizard.xml'
    ]
}
