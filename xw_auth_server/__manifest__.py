# -*- coding: utf-8 -*-
{
    'name':
        "xw_auth_server",
    'summary':
        """
        xw_auth_server
    """,
    'description':
        """
        xw_auth_server
    """,
    'author':
        "xw odoo",
    'website':
        "http://www.xwodoo.com",
    'category':
        'Apps/athentication',
    'version':
        '14.0.0.1',
    'depends': ['base'],
    'external_dependencies': {
        'python': ['xw_cbase'],
    },
    'data': [
        'security/ir.model.access.csv',
        'views/menu_root.xml',
        'views/auth_info.xml',
    ]
}
