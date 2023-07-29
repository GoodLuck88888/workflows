# -*- coding: utf-8 -*-
# from odoo import http


# class XwAuthDemo(http.Controller):
#     @http.route('/xw_auth_demo/xw_auth_demo/', auth='public')
#     def index(self, **kw):
#         return "Hello, world"

#     @http.route('/xw_auth_demo/xw_auth_demo/objects/', auth='public')
#     def list(self, **kw):
#         return http.request.render('xw_auth_demo.listing', {
#             'root': '/xw_auth_demo/xw_auth_demo',
#             'objects': http.request.env['xw_auth_demo.xw_auth_demo'].search([]),
#         })

#     @http.route('/xw_auth_demo/xw_auth_demo/objects/<model("xw_auth_demo.xw_auth_demo"):obj>/', auth='public')
#     def object(self, obj, **kw):
#         return http.request.render('xw_auth_demo.object', {
#             'object': obj
#         })
