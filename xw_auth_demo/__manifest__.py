# -*- coding: utf-8 -*-
{
    'name': "xw_auth_demo",

    'summary': """
        xw auth demo
    """,

    'description': """
        xw auth demo
    """,

    'author': "xw odoo",
    'website': "http://www.xwodoo.com",

    'category': 'Apps/athentication',
    'version': '14.0.0.1',

    'depends': ['base', 'xw_auth_base'],
    'external_dependencies': {
        'python': ['xw_cbase']
    },

    'data': [
        'security/ir.model.access.csv',
        'views/views.xml'
    ]
}
